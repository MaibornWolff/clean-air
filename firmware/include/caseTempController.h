//*****************************************//
// includes                                //
//*****************************************//
#include "caseTempController/config.h"
#include "caseTempController/temperature.h"
#include "caseTempController/fan.h"

//*****************************************//
// class                                   //
//*****************************************//
class CaseTempController
{
    private:

        // a history of the last TAU temperature errors
        int temperature_errors[TAU];

        // marks the place of the oldest info in the temperature_errors array
        int* p_oldest_temperature_error;

        // temperature sensor
        TempSensor tempSensor;

        // fan
        Fan fan;

        // counter used to check if controller should start measuring/calculating
        int sleep_check;

    public:

        CaseTempController()
        {
            this->sleep_check = 0;
        }

        int init(void);
        
        int readTemperatureAndAdjustFanSpeed(void);
};