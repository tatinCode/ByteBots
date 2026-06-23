#include "autonomy.h"

int autonomy_init(void){
    return 0;
}

const char *autonomy_next_command(void){

    return "F 120\n";
}

void autonomy_cleanup(void){

}
