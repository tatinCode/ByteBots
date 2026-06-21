/**
 * main.ino
 *
 *  Created on: 2026-06-05
 *      Author: tatin
 *      Description: Main file
 *
 */

#include <BotController.h>

BotController bot;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  bot.init();
}

void loop() {
  // put your main code here, to run repeatedly:
  bot.update();
}
