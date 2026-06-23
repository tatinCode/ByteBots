#include "autonomy.h"

#include "camera.h"

#define CAMERA_WIDTH 640
#define CAMERA_HEIGHT 480

#define AI_INPUT_WIDTH 640
#define AI_INPUT_HEIGHT 640

int autonomy_init(void){
    return camera_init(CAMERA_WIDTH, CAMERA_HEIGHT);
}

const char *autonomy_next_command(void){

    return "F 120\n";
}

void autonomy_cleanup(void){

}
