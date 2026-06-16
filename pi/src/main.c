#include "Esp32_serial/esp32_serial.h"
#include "car_controller.h"

#include <signal.h>


int main(void){
    if(car_controller_init() < 0){
        return 1;
    }

    /**
     * Starts the car controller loop
     * This function will block until the car controller loop is stopped
     * by the car controller cleanup function
     */
    car_controller_loop();

    car_controller_cleanup();

    return 0;
}
