#pragma once

int pi_mmio_init(void);
void pi_mmio_close(void);

void pi_gpio_set_pin(int pin, int value);
void pi_gpio_set_input(int pin);
void pi_gpio_set_pull_up(int pin);
int pi_gpio_read(int pin);
