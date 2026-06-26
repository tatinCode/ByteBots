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
    _back(14, 27, 26, false, 32, 25, 33, true),
    _armed(false),
    _lastMoveCommandMs(0){ }

void BotController::init(){
    _front.init();
    _back.init();
}

void BotController::update(){
    while(Serial.available() > 0){
        char c = Serial.read();

        if(c == '\n'){
            _input.trim();

            if(_input.length() > 0){
                handleCommand(_input);
            }

            _input = "";
        }
        else if(c != '\r'){
            _input += c;
        }
    }

    if(_armed && _lastMoveCommandMs > 0 &&
            millis() - _lastMoveCommandMs > COMMAND_TIMEOUT_MS){

        stop();

        _lastMoveCommandMs = 0;
    }
        

}

int BotController::parseSpeed(const String& speedText){
    if(speedText.length() == 0){
        return -1;
    }

    for(unsigned int i = 0; i < speedText.length(); i++){
        if(!isDigit(speedText[i])){
            return -1;
        }
    }

    int speed = speedText.toInt();

    return constrain(speed, 0, MAX_SPEED);
}

bool BotController::parseCommand(const String& command, String& action, String& argument){
    String parsed = command;
    parsed.trim();
    parsed.toUpperCase();

    if(parsed.length() == 0){
        return false;
    }

    int spaceIndex = parsed.indexOf(' ');
    if(spaceIndex < 0){
        action = parsed;
        argument = "";
        return true;
    }

    action = parsed.substring(0, spaceIndex);
    argument = parsed.substring(spaceIndex + 1);
    argument.trim();

    return action.length() > 0;
}

void BotController::handleCommand(String command){
    String action;
    String argument;

    if(!parseCommand(command, action, argument)){
        stop();
        _lastMoveCommandMs = 0;
        Serial.println("[ESP32 ERROR] Invalid command.");
        return;
    }

    if(action == "ARM"){
        _armed = true;
        _lastMoveCommandMs = 0;
        stop();
        Serial.println("[ESP32] Bot is armed.");
        return;
    }

    if(action == "DISARM"){
        _armed = false;
        _lastMoveCommandMs = 0;
        stop();
        Serial.println("[ESP32] Bot is disarmed.");
        return;
    }

    if(action == "S"){
        _lastMoveCommandMs = 0;
        stop();
        Serial.println("[ESP32] Stopped.");
        return;
    }

    if(!_armed){
        stop();
        Serial.println("[ESP32 ERROR] Bot is not armed.");
        return;
    }

    int speed = parseSpeed(argument);

    if(speed < 0){
        stop();
        _lastMoveCommandMs = 0;
        Serial.println("[ESP32 ERROR] Invalid speed.");
        return;
    }

    if(action == "F"){
        forward(speed);
        Serial.println("[ESP32] Forward");
    }
    else if(action == "B"){
        backward(speed);
        Serial.println("[ESP32] Backward");
    }
    else if(action == "L"){
        turn_left(speed);
        Serial.println("[ESP32] Turning left");
    }
    else if(action == "R"){
        turn_right(speed);
        Serial.println("[ESP32] Turning right");
    }
    else if(action == "SL"){
        slight_left(speed);
        Serial.println("[ESP32] Slight left");
    }
    else if(action == "SR"){
        slight_right(speed);
        Serial.println("[ESP32] Slight right");
    }
    else if(action == "XL"){
        shift_left(speed);
        Serial.println("[ESP32] Shifting left");
    }
    else if(action == "XR"){
        shift_right(speed);
        Serial.println("[ESP32] Shifting right");
    }
    else{
        stop();
        _lastMoveCommandMs = 0;
        Serial.println("[ESP32 ERROR] Invalid command.");
        return;
    }

    _lastMoveCommandMs = millis();
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
    setWheels(-speed, speed, speed, -speed);
}

void BotController::shift_right(int speed){
    setWheels(speed, -speed, -speed, speed);
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
