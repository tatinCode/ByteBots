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

static int config_width = 0;
static int config_length = 0;
static int config_channels = 0;
static int camera_initialized = 0;

static void request_completed(Request *request);

int camera_init(int width, int height){
    if(camera.isOpened()){
        camera.release();
    }

    printf("[CAMERA] Opening %s\n", CAMERA_DEVICE);

    if(!camera.open(CAMERA_DEVICE, cv::CAP_V4L2)){
        printf("[CAMERA ERROR] Failed to open %s\n", CAMERA_DEVICE);
        return -1;
    }

    camera.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('B', 'G', 'R', '3'));
    camera.set(cv::CAP_PROP_FRAME_WIDTH, width);
    camera.set(cv::CAP_PROP_FRAME_HEIGHT, height);

    printf("[CAMERA] Requested %dx%d\n", width, height);

//    if(!camera.read(frame_buffer) || frame_buffer.empty()){
//        printf("[CAMERA ERROR] Failed to read first frame\n");
//        camera.release();
//        return -1;
//    }

    int captured = 0;

    for(int i = 0; i < 10; i++){
        if(camera.read(frame_buffer) && !frame_buffer.empty()){
            captured = 1;
            break;
        }

        usleep(100 * 1000);
    }

    if(!captured){
        printf("[CAMERA ERROR] Failed to read first frame\n");
        camera.release();
        return -1;
    }

    printf("[CAMERA] Captured frame %dx%d channels=%d\n",
       frame_buffer.cols,
       frame_buffer.rows,
       frame_buffer.channels());

    camera_initialized = 1;

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

