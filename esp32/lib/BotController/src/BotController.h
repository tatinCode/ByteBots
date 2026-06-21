#pragma once

#include "L298NMotorDriver.h"

#include <Arduino.h>

class BotController {
    public:
        BotController();

        void init();
        void update();

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
        static constexpr int MAX_SPEED = 255;
        static constexpr unsigned long COMMAND_TIMEOUT_MS = 750;

        L298NMotorDriver _front;
        L298NMotorDriver _back;

        bool _armed;
        unsigned long _lastMoveCommandMs;
        String _input;

        void setWheels(int front_left, int front_right, int rear_left, int rear_right);
        void handleCommand(String command);

        int parseSpeed(const String& command);
        bool parseCommand(const String& command, String& commandName, String& commandValue);
};
