#ifndef DEFAULTS_H_
#define DEFAULTS_H_

// Serial Console Speed
const unsigned long BAUD_RATE = 115200;

// Wifi Check time
#define CHECK_WIFI_INTERVAL 30000

// Logging
#define TAG "cleanair"
// Operating Check time
#define UPDATE_OPERATING_HOURS_INTERVAL 3600000

// default SSID for AP
#define AP_SSID "CleanAirDevice"

// General FS configuration
#define FORMAT_LITTLEFS_IF_FAILED true

// Parameter file configuration
#define PARAM_FILE "/param.json"
#define OTA_URL_KEY "otaUrl"
#define VERSION_KEY "softwareVersion"

// Statistics file configuration
#define STATS_FILE "/stats.json"
#define OPERATING_HOURS_KEY "operatingHours"

#endif