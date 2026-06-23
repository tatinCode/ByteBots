#include "camera.h"

static int camera_width = 0;
static int camera_height = 0;
static int camera_initialized = 0;

int camera_init(int width, int height){
    camera_width = width;
    camera_height = height;
    camera_initialized = 1;
    return 0;
}

int camera_capture(CameraFrame *frame){
    (void)frame;
    return -1;
}

void camera_release_frame(CameraFrame *frame){
    (void)frame;
}

void camera_cleanup(){

}


