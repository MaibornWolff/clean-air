//*****************************************//
// includes                                //
//*****************************************//
#include <stdbool.h>
#include "caseTempController.h"

//*****************************************//
// functions                               //
//*****************************************//
int CaseTempController::init(void)
{
    // variables
    int i;                          // loop counter
    int temperature;                // current actual temperature
    int temperature_error;          // the current difference between wished temperature and actual temperature

    ESP_LOGD(TAG, "Configuration:");
    ESP_LOGD(TAG, "   Proportional factor:             %i", K_P);
    ESP_LOGD(TAG, "   Integral factor:                 %f", K_I);
    ESP_LOGD(TAG, "   Differential factor:             %f", K_D);
    ESP_LOGD(TAG, "   Viewed number of measurements:   %i", TAU);
    ESP_LOGD(TAG, "   Goal temperature:                %i°C", GOAL_TEMP);
    ESP_LOGD(TAG, "Initialize hardware.");
    fan.init();
    tempSensor.init();

    ESP_LOGD(TAG, "Fill the temperature_errors array with inital values.");
    for (i = 0; i < TAU; i++)
    {
        temperature = tempSensor.read();
        temperature_errors[i] =  ((temperature_error = temperature - GOAL_TEMP) > 0 ? temperature_error : 0);
    }
    p_oldest_temperature_error = &temperature_errors[0];

    return 0;
}


int CaseTempController::readTemperatureAndAdjustFanSpeed(void)
{
    if (sleep_check % SLEEP == 0)
    {
        // variables
        int i;                              // loop counter
        int temperature;                    // current actual temperature
        int temperature_error;              // the current difference between wished temperature and actual temperature
        int temperature_error_sum;          // the sum of the last TAU temperature errors
        int pr, in, di;                     // proportional, integral, and differential adjustment
        int fan_speed;                      // the fan speed that shall be set

        // read in newest temperature error, put it in place of the oldest value
        temperature = tempSensor.read();
        *p_oldest_temperature_error = temperature_error = (temperature_error = temperature - GOAL_TEMP) > 0 ? temperature_error : 0;
        ESP_LOGD(TAG, "Newest temperature error: %i°C", temperature_error);


        // set p_oldest_temperature_error to next place in array
        if (p_oldest_temperature_error == &temperature_errors[TAU - 1])
            p_oldest_temperature_error = &temperature_errors[0];
        else
            p_oldest_temperature_error++;

            
        // calculate proportional adjustment
        pr = K_P * temperature_error;
        ESP_LOGD(TAG, "Proportional adjustment: %i", pr);


        // calculate integral adjustment
        temperature_error_sum = 0;
        for (i = 0; i < TAU; i++)
            temperature_error_sum += temperature_errors[i];
        in = K_I * temperature_error_sum;
        ESP_LOGD(TAG, "Integral adjustment: %i", in);


        // calculate differential adjustment
        di = K_D * (temperature_error - *p_oldest_temperature_error);
        ESP_LOGD(TAG, "Differential adjustment: %i", di);


        // calculate and set new fan speed
        fan_speed = pr + in + di;
        if (fan_speed < 0) fan_speed = 0;
        if (fan_speed > 100) fan_speed = 100;
        ESP_LOGD(TAG, "Resulting fan speed: %i%%", fan_speed);
        fan.setSpeed(fan_speed);
    }

    sleep_check++;
    return 0;
}