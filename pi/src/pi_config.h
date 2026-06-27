#pragma once

#define BUTTON_GPIO 17

#define MOVE_SPEED 120
#define LOOP_DELAY_US (100 * 1000)
#define DEBOUNCE_US (200 * 1000)

#define ESP32_DEVICE "/dev/ttyUSB0"
#define ESP32_SERIAL_BAUD_RATE 115200

#define CAMERA_DEVICE "/dev/video0"
#define CAMERA_WIDTH 640
#define CAMERA_HEIGHT 480

#define AI_INPUT_WIDTH 640
#define AI_INPUT_HEIGHT 640

#define CAMERA_HFLIP 0
#define CAMERA_VFLIP 0
