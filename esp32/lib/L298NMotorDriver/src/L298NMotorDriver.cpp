/**
 * L298NMotorDriver.cpp
 *
 *  Created on: 2026-06-05
 *      Author: tatin
 *      Description: L298N Motor Driver
 *
 */
#include "L298NMotorDriver.h"

L298NMotorDriver::L298NMotorDriver(
        uint8_t ena, uint8_t in1, uint8_t in2, bool invertA,
        uint8_t enb, uint8_t in3, uint8_t in4, bool invertB
        )
    :_ena(ena),
    _in1(in1),
    _in2(in2),
    _invertA(invertA),
    _enb(enb),
    _in3(in3),
    _in4(in4),
    _invertB(invertB) {
}

void L298NMotorDriver::init() {
    pinMode(_in1, OUTPUT);
    pinMode(_in2, OUTPUT);
    pinMode(_in3, OUTPUT);
    pinMode(_in4, OUTPUT);

    ledcAttach(_ena, PWM_FREQ, PWM_RESOLUTION);
    ledcAttach(_enb, PWM_FREQ, PWM_RESOLUTION);

    stop();
}

void L298NMotorDriver::setMotorA(int speed){
    if(_invertA){
        speed = -speed;
    }

    setMotor(_ena, _in1, _in2, speed);
}

void L298NMotorDriver::setMotorB(int speed){
    if(_invertB){
        speed = -speed;
    }

    setMotor(_enb, _in3, _in4, speed);
}

void L298NMotorDriver::stop(){
    setMotorA(0);
    setMotorB(0);
}

void L298NMotorDriver::setMotor(uint8_t enable_pin, uint8_t in1, uint8_t in2, int speed){
    speed = constrain(speed, -MAX_SPEED, MAX_SPEED);

    if(speed > 0){
        digitalWrite(in1, HIGH);
        digitalWrite(in2, LOW);
        ledcWrite(enable_pin, speed);
    }
    else if(speed < 0){
        digitalWrite(in1, LOW);
        digitalWrite(in2, HIGH);
        ledcWrite(enable_pin, -speed);
    }
    else{
        digitalWrite(in1, LOW);
        digitalWrite(in2, LOW);
        ledcWrite(enable_pin, 0);
    }
}

