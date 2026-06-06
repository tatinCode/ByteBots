#include "BotController.h"
#include "L298NMotorDriver.h"

/**
 * Wheel layout:
 * front:
 * A: left
 * B: right
 *
 * rear:
 * A: right
 * B: left
 */

BotController::BotController()
    : _front(19, 18, 5, true, 4, 17, 16, false),
    _back(14, 27, 26, true, 32, 25, 33, false) {

}

void BotController::init(){
    _front.init();
    _back.init();
}

void BotController::forward(int speed){
    setWheels(speed, speed, speed, speed);
}

void BotController::backward(int speed){
    setWheels(-speed, -speed, -speed, -speed);
}

void BotController::turn_left(int speed){
    setWheels(-speed, speed, -speed, speed);
}

void BotController::turn_right(int speed){
    setWheels(speed, -speed, speed, -speed);
}

void BotController::slight_left(int speed){
    setWheels(speed * 0.4, speed, speed * 0.4, speed);
}

void BotController::slight_right(int speed){
    setWheels(speed, speed * 0.4, speed, speed * 0.4);
}

void BotController::shift_left(int speed){
    setWheels(-speed, -speed, speed, speed);
}

void BotController::shift_right(int speed){
    setWheels(speed, speed, -speed, -speed);
}

void BotController::setWheels(int front_left, int front_right, int rear_left, int rear_right){
    _front.setMotorA(front_left);
    _front.setMotorB(front_right);
    _back.setMotorB(rear_left);
    _back.setMotorA(rear_right);
}

void BotController::stop(){
    _front.stop();
    _back.stop();
}
