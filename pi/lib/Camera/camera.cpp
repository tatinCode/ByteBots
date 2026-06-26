#include "camera.h"

#include <opencv2/opencv.hpp>

#include <stdio.h>

#define CAMERA_DEVICE "/dev/video0"

static cv::VideoCapture camera;
static cv::Mat frame_buffer;

static int camera_width = 0;
static int camera_height = 0;
static int camera_initialized = 0;

int camera_init(int width, int height){
    if(camera.isOpened()){
        camera.release();
    }

    printf("[CAMERA] Opening %s\n", CAMERA_DEVICE);

    if(!camera.open(CAMERA_DEVICE, cv::CAP_V4L2)){
        printf("[CAMERA ERROR] Failed to open %s\n", CAMERA_DEVICE);
        return -1;
    }

    camera.set(cv::CAP_PROP_FRAME_WIDTH, width);
    camera.set(cv::CAP_PROP_FRAME_HEIGHT, height);

    printf("[CAMERA] Requested %dx%d\n", width, height);

    if(!camera.read(frame_buffer) || frame_buffer.empty()){
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


