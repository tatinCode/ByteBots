#include "autonomy.h"

#include "camera.h"
#include "pi_config.h"

int autonomy_init(void){
    return camera_init(CAMERA_WIDTH, CAMERA_HEIGHT);
}

const char *autonomy_next_command(void){

    return "F 120\n";
}

void autonomy_cleanup(void){
    camera_cleanup();
}
