#include <Arduino.h>
#line 1 "/home/tatin/projects/ardu/motor/esp32/main/main.ino"
/**
 * main.ino
 *
 *  Created on: 2026-06-05
 *      Author: tatin
 *      Description: Main file
 *
 */
#include <L298NMotorDriver.h>

#define MAX_SPEED 180

L298NMotorDriver front(19, 18, 5, true, 4, 17, 16, false);
L298NMotorDriver back(14, 27, 26, true, 32, 25, 33, false);

#line 16 "/home/tatin/projects/ardu/motor/esp32/main/main.ino"
void setup();
#line 23 "/home/tatin/projects/ardu/motor/esp32/main/main.ino"
void loop();
#line 16 "/home/tatin/projects/ardu/motor/esp32/main/main.ino"
void setup() {
  // put your setup code here, to run once:
  front.init();
  back.init();

}

void loop() {
  // put your main code here, to run repeatedly:
  front.setMotorA(MAX_SPEED);
  front.setMotorB(MAX_SPEED);
  back.setMotorA(MAX_SPEED);
  back.setMotorB(MAX_SPEED);
  delay(1000);

  front.stop();
  back.stop();
  delay(2 * 1000);

  front.setMotorA(-MAX_SPEED);
  front.setMotorB(-MAX_SPEED);
  back.setMotorA(-MAX_SPEED);
  back.setMotorB(-MAX_SPEED);
  delay(1000);
  
  front.stop();
  back.stop();
  delay(5 * 1000);

  /**
    front.stop();
    back.stop();
    delay(2000);

    front.setMotorA(180);
    delay(2000);
    front.stop();
    delay(2000);

    front.setMotorB(180);
    delay(2000);
    front.stop();
    delay(2000);

    back.setMotorA(180);
    delay(2000);
    back.stop();
    delay(2000);

    back.setMotorB(180);
    delay(2000);
    back.stop();
    delay(5000);
    */
}

