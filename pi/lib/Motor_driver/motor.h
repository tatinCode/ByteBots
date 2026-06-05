#pragma once

enum MotorAction{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    SHIFT_LEFT,
    SHIFT_RIGHT
};

int motor_init();
void motor_update(MotorAction action);
int motor_cleanup();



