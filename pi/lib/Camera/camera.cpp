#include "camera.h"

#include <opencv2/opencv.hpp>

static cv::VideoCapture camera;
static cv::Mat frame_buffer;

static int camera_width = 0;
static int camera_height = 0;
static int camera_initialized = 0;

int camera_init(int width, int height){
    if(camera.isOpened()){
        camera.release();
    }

    if(!camera.open(0, cv::CAP_V4L2)){
        return -1;
    }

    camera.set(cv::CAP_PROP_FRAME_WIDTH, width);
    camera.set(cv::CAP_PROP_FRAME_HEIGHT, height);

    if(!camera.read(frame_buffer) || frame_buffer.empty()){
        camera.release();
        return -1;
    }

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

    return -1;
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


