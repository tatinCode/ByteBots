#include "camera.h"

static int camera_width = 0;
static int camera_height = 0;
static int camera_initialized = 0;

int camera_init(int width, int height){
    camera_width = width;
    camera_height = height;
    camera_initialized = 1;

    (void)camera_width;
    (void)camera_height;

    return 0;
}

int camera_capture(CameraFrame *frame){
    if(!camera_initialized || frame == nullptr){
        return -1;
    }

    frame->width = camera_width;
    frame->height = camera_height;
    frame->channels = 0;
    frame->data = nullptr;

    return -1;
}

void camera_release_frame(CameraFrame *frame){
    (void)frame;
}

void camera_cleanup(){
    camera_width = 0;
    camera_height = 0;
    camera_initialized = 0;
}


