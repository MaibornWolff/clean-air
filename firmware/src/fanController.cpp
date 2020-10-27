#include "Arduino.h"
#include "fanController.h"

// The fans target speed.
int fanSpeedToRunAt = -1;

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
void FanController::configure(/*void (*speedHandler)(int, int, int, int)*/)
{
    //localSpeedHandler = speedHandler;

    ledcAttachPin(FAN_PWM, 0);
    ledcSetup(0, FAN_FREQ, FAN_RES);

    //Set fan speed to 100% to ensure good ventilation during a possible calibration process
    fanSpeedToRunAt = 100;
}

// TODO: this needs to run on a different thread to be able to constantly monitor the fans speed
void FanController::emitFanSpeed()
{
    localSpeedHandler(fan_1_speed, fan_2_speed, fan_3_speed, fan_4_speed);
}

// Set fan speed in percent - 0 is off, 1-100 is minimum to maximum speed
void FanController::setSpeed(int speed)
{
    ledcWrite(0, (speed == 0) ? 0 : map(speed, 1, 100, 85, 255));
    fanSpeedToRunAt = speed;
}