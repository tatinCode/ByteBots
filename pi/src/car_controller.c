#include "Esp32_serial/esp32_serial.h"
#include "pi_config.h"

int car_controller_init() {
    int fd = esp32_serial_open(ESP32_DEVICE);

    pi_mmio_init();
    pi_gpio_set_input(INPUT_GPIO);

    return 0;
}

void car_controller_loop() {

}

void car_controller_cleanup() {

}
