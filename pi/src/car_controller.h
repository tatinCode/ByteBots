#pragma once

#include <signal.h>

int car_controller_init();
void car_controller_loop(volatile sig_atomic_t *keep_running);
void car_controller_cleanup();
