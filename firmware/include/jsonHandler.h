#ifndef JSON_HANDLER_H_
#define JSON_HANDLER_H_

#include <ArduinoJson.h>

#define FORMAT_LITTLEFS_IF_FAILED true
#define DOC_SIZE 1024

// Parameter file configuration
#define PARAM_FILE "/param.json"
#define OTA_URL_KEY "otaUrl"
#define VERSION_KEY "softwareVersion"

// Statistics file configuration
#define STATS_FILE "/stats.json"
#define OPERATING_HOURS_KEY "operatingHours"
#define FAN_SPEED_KEY "fanSpeed"

class JsonHandler
{
public:
    StaticJsonDocument<DOC_SIZE> statistics;
    StaticJsonDocument<DOC_SIZE> parameter;

    JsonHandler();

    virtual void loadStatistics();
    virtual void loadParameter();

    virtual void storeStatistics();
    virtual void storeParameter();
};

#endif