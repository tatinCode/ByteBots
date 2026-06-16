#include "car_controller.h"

#include <signal.h>

volatile sig_atomic_t keep_running = 1;

void sigint_handler(int signum);

int main(void){
    signal(SIGINT, sigint_handler);
    signal(SIGTERM, sigint_handler);

    if(car_controller_init() < 0){
        return 1;
    }

    /**
     * Starts the car controller loop
     * This function will block until the car controller loop is stopped
     * by the car controller cleanup function
     */
    car_controller_loop(&keep_running);

    car_controller_cleanup();

    return 0;
}

void sigint_handler(int signum){
    (void)signum;
    keep_running = 0;
}
