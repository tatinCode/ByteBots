#pragma once

#include <Arduino.h>

class L298NMotorDriver {
  public:
    L298NMotorDriver(
            uint8_t ena, uint8_t in1, uint8_t in2, bool invertA,
            uint8_t enb, uint8_t in3, uint8_t in4, bool invertB
            );
    void init();
    void setMotorA(int speed);
    void setMotorB(int speed);
    void stop();

  private:
    static constexpr uint32_t PWM_FREQ = 1000;
    static constexpr uint8_t PWM_RESOLUTION = 8;
    static constexpr int MAX_SPEED = 255;

    //right
    uint8_t _ena;
    uint8_t _in1;
    uint8_t _in2;
    bool _invertA;

    //left
    uint8_t _enb;
    uint8_t _in3;
    uint8_t _in4;
    bool _invertB;

    void setMotor(uint8_t enable_pin, uint8_t in1, uint8_t in2, int speed);
};

