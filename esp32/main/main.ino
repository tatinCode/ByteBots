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

void setup() {
  // put your setup code here, to run once:
  front.init();
  back.init();

}

void loop() {
  // put your main code here, to run repeatedly:

}
