#include "camera.h"
#include "pi_config.h"

#include <libcamera/camera.h>
#include <libcamera/camera_manager.h>
#include <libcamera/framebuffer.h>
#include <libcamera/framebuffer_allocator.h>
#include <libcamera/formats.h>
#include <libcamera/request.h>
#include <libcamera/stream.h>

#include <algorithm>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <vector>
#include <sys/mman.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <chrono>

using namespace libcamera;

static std::unique_ptr<CameraManager> camera_manager;
static std::shared_ptr<Camera> camera_device;
static std::unique_ptr<CameraConfiguration> camera_config;
static std::unique_ptr<FrameBufferAllocator> allocator;
static Stream *camera_stream = nullptr;
static std::unique_ptr<Request> capture_request;

static void *mapped_buffer = MAP_FAILED;
static size_t mapped_buffer_size = 0;

static std::mutex capture_mutex;
static std::condition_variable capture_cv;
static bool frame_ready = false;
static bool request_started = false;
static Request *completed_request = nullptr;

static int configured_width = 0;
static int configured_height = 0;
static int configured_channels = 0;
static int camera_initialized = 0;

static std::vector<uint8_t> frame_storage;

static void request_completed(Request *request);

int camera_init(int width, int height){
    printf("[CAMERA] Initializing libcamera\n");

    camera_manager = std::make_unique<CameraManager>();

    if(camera_manager->start() < 0){
        printf("[CAMERA ERROR] Failed to start CameraManager\n");
        camera_cleanup();
        return -1;
    }

    if(camera_manager->cameras().empty()){
        printf("[CAMERA ERROR] No cameras found\n");
        camera_cleanup();
        return -1;
    }

    camera_device = camera_manager->cameras()[0];

    if(camera_device->acquire() < 0){
        printf("[CAMERA ERROR] Failed to acquire camera\n");
        camera_cleanup();
        return -1;
    }

    printf("[CAMERA] Found camera %s\n", camera_device->id().c_str());

    camera_config = camera_device->generateConfiguration({
            StreamRole::Viewfinder
            });

    if(!camera_config || camera_config->empty()){
        printf("[CAMERA ERROR] Failed to get camera configuration\n");
        camera_cleanup();
        return -1;
    }
    
    StreamConfiguration &stream_config = camera_config->at(0);
    stream_config.size.width = width;
    stream_config.size.height = height;
    stream_config.pixelFormat = formats::RGB888;
    stream_config.bufferCount = 1;

    CameraConfiguration::Status validation = camera_config->validate();

    printf("[CAMERA] Validated format: %s %dx%d\n",
       stream_config.pixelFormat.toString().c_str(),
       stream_config.size.width,
       stream_config.size.height);

    if(validation == CameraConfiguration::Status::Invalid){
        printf("[CAMERA ERROR] Invalid camera configuration\n");
        camera_cleanup();
        return -1;
    }

    if(stream_config.pixelFormat == formats::RGB888){
        configured_channels = 3;
    }
    else{
        printf("[CAMERA ERROR] Unsupported pixel format for current CameraFrame API: %s\n",
                stream_config.pixelFormat.toString().c_str());
        camera_cleanup();
        return -1;
    }

    if(camera_device->configure(camera_config.get()) < 0){
        printf("[CAMERA ERROR] Failed to configure camera\n");
        camera_cleanup();
        return -1;
    }

    configured_width = stream_config.size.width;
    configured_height = stream_config.size.height;

    printf("[CAMERA] Camera configured %dx%d RGB888\n", configured_width, configured_height);

    camera_stream = stream_config.stream();

    allocator = std::make_unique<FrameBufferAllocator>(camera_device);

    if(allocator->allocate(camera_stream) < 0){
        printf("[CAMERA ERROR] Failed to allocate framebuffer\n");
        camera_cleanup();
        return -1;
    }

    const std::vector<std::unique_ptr<FrameBuffer>> &buffers = allocator->buffers(camera_stream);

    if(buffers.empty()){
        printf("[CAMERA ERROR] No frame buffers allocated\n");
        camera_cleanup();
        return -1;
    }

    FrameBuffer *buffer = buffers[0].get();

    if(buffer->planes().size() != 1){
        printf("[CAMERA ERROR] Frame buffer has no planes\n");
        camera_cleanup();
        return -1;
    }

    const FrameBuffer::Plane &plane = buffer->planes()[0];

    mapped_buffer_size = plane.length;
    mapped_buffer = mmap(NULL, mapped_buffer_size, PROT_READ | PROT_WRITE, MAP_SHARED, plane.fd.get(), plane.offset);

    if(mapped_buffer == MAP_FAILED){
        printf("[CAMERA ERROR] Failed to mmap camera buffer\n");
        mapped_buffer = nullptr;
        camera_cleanup();
        return -1;
    }
    
    capture_request = camera_device->createRequest();

    if(!capture_request){
        printf("[CAMERA ERROR] Failed to create request\n");
        camera_cleanup();
        return -1;
    }
    
    if(capture_request->addBuffer(camera_stream, buffer) < 0){
        printf("[CAMERA ERROR] Failed to add buffer to request\n");
        camera_cleanup();
        return -1;
    }

    camera_device->requestCompleted.connect(request_completed);

    if(camera_device->start() < 0){
        printf("[CAMERA ERROR] Failed to start camera\n");
        camera_cleanup();
        return -1;
    }

    frame_storage.resize((size_t)configured_width * configured_height * configured_channels);

    camera_initialized = 1;

    printf("[CAMERA] Camera Started\n");

    return 0;
}

int camera_capture(CameraFrame *frame){
    if(!camera_initialized || frame == nullptr){
        return -1;
    }

    {
        std::lock_guard<std::mutex> lock(capture_mutex);
        frame_ready = false;
        completed_request = nullptr;
    }

    if(request_started){
        capture_request->reuse(Request::ReuseBuffers);
    }

    request_started = true;

    if(camera_device->queueRequest(capture_request.get()) < 0){
        printf("[CAMERA ERROR] Failed to queue capture request\n");
        return -1;
    }

        

    {
        std::unique_lock<std::mutex> lock(capture_mutex);
        bool completed = capture_cv.wait_for(lock, std::chrono::seconds(2), []{
                return frame_ready;
                });
        if(!completed){
            printf("[CAMERA ERROR] Timeout waiting for capture\n");
            return -1;
        }
    }


    if(completed_request == nullptr){
        return -1;
    }

    size_t expected_size = frame_storage.size();
    size_t copy_size = std::min(expected_size, mapped_buffer_size);

    memcpy(frame_storage.data(), mapped_buffer, copy_size);

    frame->width = configured_width;
    frame->height = configured_height;
    frame->channels = configured_channels;
    frame->data = frame_storage.data();

    return 0;
}

void camera_release_frame(CameraFrame *frame){
    (void)frame;
}

void camera_cleanup(){
    printf("[CAMERA] Cleaning up camera\n");

    if(camera_device){
        if(camera_initialized){
            camera_device->stop();
            camera_device->requestCompleted.disconnect(request_completed);
        }

        camera_device->release();
        camera_device.reset();
    }

    capture_request.reset();

    if(mapped_buffer != nullptr && mapped_buffer != MAP_FAILED){
        munmap(mapped_buffer, mapped_buffer_size);
        mapped_buffer = nullptr;
        mapped_buffer_size = 0;
    }

    allocator.reset();

    if(camera_manager){
        camera_manager->stop();
        camera_manager.reset();
    }

    frame_storage.clear();

    camera_stream = nullptr;
    camera_config.reset();

    camera_initialized = 0;
    request_started = false;
}

static void request_completed(Request *request){
    if(request->status() == Request::RequestCancelled){
        return;
    }

    std::lock_guard<std::mutex> lock(capture_mutex);
    completed_request = request;
    frame_ready = true;
    capture_cv.notify_one();
}

