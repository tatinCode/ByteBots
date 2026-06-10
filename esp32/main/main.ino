/**
 * main.ino
 *
 *  Created on: 2026-06-05
 *      Author: tatin
 *      Description: Main file
 *
 */

#include <BotController.h>

#define MAX_SPEED 180

BotController bot;

void setup() {
  // put your setup code here, to run once:
  bot.init();
}

void loop() {
  // put your main code here, to run repeatedly:
  bot.update();
}
