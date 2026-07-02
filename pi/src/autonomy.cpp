#include "autonomy.h"
#include "camera.h"
#include "pi_config.h"

#include <stdio.h>

static int frame_count = 0;

int autonomy_init(void){
    if(camera_init(CAMERA_WIDTH, CAMERA_HEIGHT) < 0){
        return -1;
    }

    return 0;
}

const char *autonomy_next_command(void){
    CameraFrame frame;

    if(camera_capture(&frame) < 0){
        printf("[AUTONOMY ERROR] camera_capture failed\n");
        return "S\n";
    }

    frame_count++;

    if(frame_count % 30 == 0){
        printf("[AUTONOMY] Captured frame %dx%d channels=%d\n",
                frame.width,
                frame.height,
                frame.channels);

    }

    camera_release_frame(&frame);

    return "F 120\n";
}

void autonomy_cleanup(void){
    camera_cleanup();
}
