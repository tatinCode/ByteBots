#pragma once

int esp32_serial_open(const char *device);
void esp32_serial_close(int fd);
int esp32_serial_send(int fd, const char *command);
