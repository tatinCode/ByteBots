#pragma once

int esp32_serial_open(void);
void esp32_serial_close(void);

int esp32_serial_send(int fd, const char *command);
