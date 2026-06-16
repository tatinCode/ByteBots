#include "car_controller.h"

#include "esp32_serial.h"
#include "pi_mmio.h"
#include "pi_config.h"

#include <signal.h>
#include <stdint.h>
#include <time.h>



int car_controller_init() {
    int esp32_fd = esp32_serial_open(ESP32_DEVICE);
    if(esp32_fd < 0){
        return -1;
    }

    if(pi_mmio_init() < 0){
        esp32_serial_close(esp32_fd);
        esp32_fd = -1;
        return -1;
    }

    pi_gpio_set_input(BUTTON_GPIO);
    pi_gpio_set_pull_up(BUTTON_GPIO);

    esp32_serial_send(esp32_fd, "DISARM\n");

    return 0;
}

void car_controller_loop(sigatomic_t *keep_running) {
    int was_pressed = 0;
    uint64_t last_button_press_us = 0;

    while(keep_running) {
        uint64_t now_us = get_time_us();
        int pressed = pi_gpio_read(BUTTON_GPIO) == 0;
        int debounce_elapsed = now_us - last_button_press_us > DEBOUNCE_TIME_US;

        if(pressed && !was_pressed && debounce_elapsed){
            last_button_press_us = now_us;

            robot_running = !robot_running;

            if(robot_running){
                esp32_serial_send(esp32_fd, "ARM\n");
            }
            else{
                esp32_serial_send(esp32_fd, "S\n");
                esp32_serial_send(esp32_fd, "DISARM\n");
            }
        }

        was_pressed = pressed;

        if(robot_running){
            esp32_serial_send(esp32_fd, "F 120\n");
        }

        usleep(LOOP_DELAY_US);
    }

}

void car_controller_cleanup() {

}

static uint64_t get_time_us() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);

    return ((uint64_t)ts.tv_sec * 1000000) + ((uint64_t)ts.tv_nsec / 1000);
}
