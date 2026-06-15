#include "Esp32_serial/esp32_serial.h"

#include <stdio.h>

int main(void){
    int fd = esp32_serial_open();

    pi_mmio_init();

    return 0;
}
