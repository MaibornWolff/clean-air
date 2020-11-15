//*****************************************//
// includes                                //
//*****************************************//
#include <DS18B20.h>
#include "config.h"


//*****************************************//
// class                                   //
//*****************************************//
class TempSensor
{
    private:

        OneWire* oneWire;

        DS18B20* sensors;

    public:

        TempSensor()
        {
            oneWire = new OneWire(TEMP_GPIO);
            sensors = new DS18B20(oneWire);
        }

        void init(void);

        int read(void);
};