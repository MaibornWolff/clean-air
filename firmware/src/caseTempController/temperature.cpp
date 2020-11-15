//*****************************************//
// includes                                //
//*****************************************//
#include "caseTempController/temperature.h"

//*****************************************//
// functions                               //
//*****************************************//
void TempSensor::init(void)
{
    (*sensors).begin();
    return;
}


int TempSensor::read(void)
{
        int temperature = 0;
        (*sensors).requestTemperatures();
        temperature = (*sensors).getTempC();
        ESP_LOGD(TAG, "Newest temperature: %i°C", temperature);
        return temperature;
}