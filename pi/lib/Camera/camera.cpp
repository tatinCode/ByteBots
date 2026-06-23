#include "camera.h"

int camera_init(int width, int height){
    (void)width;
    (void)height;
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


