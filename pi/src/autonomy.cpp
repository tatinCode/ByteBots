#include "autonomy.h"
#include "camera.h"
#include "pi_config.h"

#include <stdio.h>

int autonomy_init(void){
    if(camera_init camera_init(CAMERA_WIDTH, CAMERA_HEIGHT) < 0){
        return -1;
    }

    CameraFrame frame;

    if(camera_capture(&frame) < 0){
        printf("[AUTONOMY ERROR] Failed to capture frame\n");
        camera_cleanup();
        return -1;
    }

    printf("[AUTONOMY] Test frame captured: %dx%d channels:%d\n",
            frame.width,
            frame.height,
            frame.channels);

    camera_release_frame(&frame);

    return 0;
}

const char *autonomy_next_command(void){

    return "F 120\n";
}

void autonomy_cleanup(void){
    camera_cleanup();
}
