#ifndef JSON_HANDLER_H_
#define JSON_HANDLER_H_

#include <ArduinoJson.h>

#define FORMAT_LITTLEFS_IF_FAILED true
#define DOC_SIZE 512

// Parameter file configuration
static String PARAM_FILE{"/param.json"};
static String OTA_URL_KEY{"otaUrl"};
static String VERSION_KEY{"softwareVersion"};

// Statistics file configuration
static String STATS_FILE{"/stats.json"};
static String OPERATING_HOURS_KEY{"operatingHours"};
static String FAN_SPEED_KEY{"fanSpeed"};

class JsonHandler
{
public:
    StaticJsonDocument<DOC_SIZE> statistics;
    StaticJsonDocument<DOC_SIZE> parameter;

    void setup();

    void loadStatistics();
    void loadParameter();

    void storeStatistics();
    void storeParameter();
};

#endif