//*****************************************//
// includes                                //
//*****************************************//
#include "caseTempController/fan.h"


//*****************************************//
// defines                                 //
//*****************************************//
#define FREQUENCY   25000   // the PWM frequency (fan runs best at 25kHz)
#define RESOLUTION  8       // the duty cycle resolution in bits (8 means 255 steps)

//*****************************************//
// functions                               //
//*****************************************//

void Fan::init(void)
{
    ledcSetup(0, FREQUENCY, RESOLUTION);
    ledcAttachPin(FAN_GPIO, FAN_PWM_CHANNEL);
    setSpeed(0);
    return;
}


void Fan::setSpeed(int fan_speed)
{
    ledcWrite(FAN_PWM_CHANNEL, (int)(fan_speed * (255 / 100)));
}