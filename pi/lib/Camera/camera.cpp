#include "camera.h"
#include "pi_config.h"

#include <opencv2/opencv.hpp>

#include <libcamera/span/span.h>
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

using namespace libcamera;

static std::unique_ptr<CameraManager> camera_manager;
static std::shared_ptr<Camera> camera_device;
static std::unique_ptr<CameraConfiguration> camera_config;
static std::unique_ptr<FrameBufferAllocator> allocator;
static Stream *camera_stream = nullptr;
static std::unique_ptr<Request> capture_request;

static void *mapped_buffer = MAP_FAILED;
static size_t mapped_buffer_size = 0;

static std::mutex caputure_mutex;
static std::condition_variable capture_cv;
static bool frame_ready = false;
static bool request_started = false;
static Request *completed_request = nullptr;

static int configured_width = 0;
static int configured_height = 0;
static int configured_channels = 0;
static int camera_initialized = 0;

static void request_completed(Request *request);

int camera_init(int width, int height){
    printf("[CAMERA] Initializing libcamera\n");

    camera_manager = std::make_unique<CameraManager>();

    if(camera_manager->start() < 0){
        printf("[CAMERA ERROR] Failed to start CameraManager\n");
        return -1;
    }

    if(camera_manager->cameras().empty()){
        printf("[CAMERA ERROR] No cameras found\n");
        return -1;
    }

    camera_device = camera_manager->cameras()[0];

    printf("[CAMERA] Found camera %s\n", camera_device->id().c_str());

    if(!camera_config || camera_config->empty()){
        printf("[CAMERA ERROR] Failed to get camera configuration\n");
        return -1;
    }
    
    StreamConfiguration &stream_config = camera_config->at(0);
    stream_config.size.width = width;
    stream_config.size.height = height;
    stream_config.pixelFormat = formats::RGB888;
    stream_config.bufferCount = 1;

    CameraConfiguration::Status validation = camera_config->validate();

    if(validation == CameraConfiguration::Status::Invalid){
        printf("[CAMERA ERROR] Invalid camera configuration\n");
        return -1;
    }

    if(stream_config.pixelFormat != formats::RGB888){
        printf("[CAMERA ERROR] RGB888 not accepted by camera pipeline\n");
        printf("[CAMERA] actual pixel format: %s\n", stream_config.pixelFormat.to_string().c_str());
        return -1;
    }

    if(camera_device->configure(camera_config.get()) < 0){
        printf("[CAMERA ERROR] Failed to configure camera\n");
        return -1;
    }

    configured_width = stream_config.size.width;
    configured_height = stream_config.size.height;
    configured_channels = 3;

    printf("[CAMERA] Camera configured %dx%d RGB888\n", configured_width, configured_height);

    camera_stream = stream_config.createStream();

    allocator = std::make_unique<FrameBufferAllocator>(camera_device);

    if(allocator->allocate(camera_stream) < 0){
        printf("[CAMERA ERROR] Failed to allocate framebuffer\n");
        return -1;
    }

    const std::vector<std::unique_ptr<FrameBuffer>> &buffers = allocator->buffers(camera_stream);

    if(buffers.empty()){
        printf("[CAMERA ERROR] No frame buffers allocated\n");
        return -1;
    }

    FrameBuffer &buffer = buffers[0].get();

    if(buffer->planes().size() != 1){
        printf("[CAMERA ERROR] Frame buffer has no planes\n");
        return -1;
    }

    const FrameBuffer::Plane &plane = buffer->planes()[0];

    mapped_buffer_size = plane.length;
    mapped_buffer = mmap(NULL, mapped_buffer_size, PROT_READ | PROT_WRITE, MAP_SHARED, plane.fd.get(), plane.offset);

    if(mapped_buffer == MAP_FAILED){
        printf("[CAMERA ERROR] Failed to mmap camera buffer\n");
        mapped_buffer = nullptr;
        return -1;
    }
    
    capture_request = camera_device->createRequest();

    if(!capture_request){
        printf("[CAMERA ERROR] Failed to create request\n");
        return -1;
    }
    
    if(capture_request->addBuffer(camera_stream, buffer) < 0){
        printf("[CAMERA ERROR] Failed to add buffer to request\n");
        return -1;
    }

    camera_device->requestCompleted.connect(request_complete);

    if(capture_request->start() < 0){
        printf("[CAMERA ERROR] Failed to start request\n");
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

    if(!camera.read(frame_buffer) || frame_buffer.empty()){
        return -1;
    }

    frame->width = frame_buffer.cols;
    frame->height = frame_buffer.rows;
    frame->channels = frame_buffer.channels();
    frame->data = frame_buffer.data;

    return 0;
}

void camera_release_frame(CameraFrame *frame){
    (void)frame;
}

void camera_cleanup(){
    if(camera.isOpened()){
        camera.release();
    }

    frame_buffer.release();
    camera_initialized = 0;
}


static void request_completed(Request *request){
    if(request->status() == Request::RequestCancelled){
        return;
    }

    std::lock_guard<std::mutex> lock(caputure_mutex);
    completed_request = request;
    frame_ready = true;
    capture_cv.notify_one();
}

