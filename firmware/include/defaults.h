#ifndef DEFAULTS_H_
#define DEFAULTS_H_

// Serial Console Speed
const unsigned long BAUD_RATE = 115200;

// Wifi Check time
#define CHECK_WIFI_INTERVAL 30000

// Logging
#define TAG "cleanair"

// default SSID for AP
#define AP_SSID "CleanAirDevice"

// Parameter file configuration
#define PARAM_FILE "/param.json"
#define OTA_URL_KEY "otaUrl"
#define VERSION_KEY "softwareVersion"

#define FORMAT_LITTLEFS_IF_FAILED true

#endif