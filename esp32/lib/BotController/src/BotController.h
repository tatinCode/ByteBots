#pragma once

#include <Arduino.h>

class BotController {
    public:
        BotController();

        void init();

        void forward(int speed);
        void backward(int speed);
        void turn_left(int speed);
        void turn_right(int speed);
        void slight_left(int speed);
        void slight_right(int speed);
        void shift_left(int speed);
        void shift_right(int speed);

        void stop();

    private:
        L298NMotorDriver _front;
        L298NMotorDriver _back;

        void setWheels(int front_left, int front_right, int rear_left, int rear_right);
};
