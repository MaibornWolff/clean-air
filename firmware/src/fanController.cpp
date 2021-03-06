#include "Arduino.h"
#include "fanController.h"
#include "rotary.h"

// The fans target speed.
int fanSpeedToRunAt;

// The speed of fan 1.
int fan_1_speed = -1;

// The speed of fan 2.
int fan_2_speed = -1;

// The speed of fan 3.
int fan_3_speed = -1;

// The speed of fan 4.
int fan_4_speed = -1;

// Handler to emit the fans speed.
void (*localSpeedHandler)(int, int, int, int);

// Configure fan related stuff during setup.
void FanController::setup(/*void (*speedHandler)(int, int, int, int)*/)
{
    //localSpeedHandler = speedHandler;
    ledcAttachPin(FAN_PWM, FAN_MAIN_CHANNEL);
    ledcSetup(FAN_MAIN_CHANNEL, FAN_FREQ, FAN_RES);
}

// TODO: this needs to run on a different thread to be able to constantly monitor the fans speed
void FanController::emitFanSpeed()
{
    localSpeedHandler(fan_1_speed, fan_2_speed, fan_3_speed, fan_4_speed);
}

// Set fan speed in percent - 0 is off, 1-100 is minimum to maximum speed
void FanController::setSpeed(int speed)
{
    ledcWrite(FAN_MAIN_CHANNEL, speed);
    fanSpeedToRunAt = speed;
}