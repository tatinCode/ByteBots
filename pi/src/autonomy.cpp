#include "autonomy.h"
#include "camera.h"
#include "pi_config.h"

#include <stdio.h>

int autonomy_init(void){
    if(camera_init(CAMERA_WIDTH, CAMERA_HEIGHT) < 0){
        return -1;
    }

    if(vision_init() < 0){
        return -1;
    }

    return 0;
}

const char *autonomy_next_command(void){
    CameraFrame frame;

    if(camera_capture(&frame) < 0){
        return "S\n";
    }

    VisionResult result = vision_analyze(&frame);
    camera_release_frame(&frame);

    if(!result.valid){
        printf("[AUTONOMY] Invalid vision frame brightness: %.2f\n", result.brightness);
        return "S\n";
    }

    return "F 120\n";
}

void autonomy_cleanup(void){
    camera_cleanup();
    vision_cleanup();
}
