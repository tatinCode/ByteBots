#include "Esp32_serial/esp32_serial.h"
#include "car_controller.h"

#include <signal.h>


int main(void){
    car_controller_init();

    car_controller_loop();

    car_controller_cleanup();

    return 0;
}
