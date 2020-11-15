//*****************************************//
// includes                                //
//*****************************************//
#include "Arduino.h"
#include "config.h"


//*****************************************//
// class                                   //
//*****************************************//
class Fan
{
    public:

        Fan() {}

        void init(void);

        void setSpeed(int fan_speed);
};