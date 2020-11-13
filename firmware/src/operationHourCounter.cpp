#include "esp_log.h"
#include "jsonHandler.h"
#include "operationHourCounter.h"

// Variables
// The unit of time during which the last check for an update has taken place. Is dependent on the update interval.
u_long lastHourUpdatedAt = 0;

u_long operatingHours = 0;

// Define the jsonHandler.
extern JsonHandler jsonHandler;

// Constructor.
void OperationHourCounter::setup(JsonHandler handler)
{
    jsonHandler = handler;
    this->loadHours();
}

void OperationHourCounter::loadHours()
{
    u_long opHours = jsonHandler.statistics[OPERATING_HOURS_KEY].as<u_long>();
    if (opHours)
    {
        operatingHours = opHours;
    }
    else
    {
        operatingHours = 0;
    }
}

void OperationHourCounter::updateHours()
{
    operatingHours++;

    ESP_LOGV(TAG, "Uptime: %d ms, total operating time: %lu h.", millis(), operatingHours);

    jsonHandler.statistics[OPERATING_HOURS_KEY] = operatingHours;
    jsonHandler.storeStatistics();
}

void OperationHourCounter::countOperationHours()
{
    u_long upTimeInHours = round(millis() / UPDATE_OPERATING_HOURS_INTERVAL);

    if (upTimeInHours != lastHourUpdatedAt)
    {
        updateHours();
        lastHourUpdatedAt = upTimeInHours;
    }
}