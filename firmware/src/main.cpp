/**
 * Clean Air Main.
 */

#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE
#include <AutoConnect.h>
#include <FS.h>
#include <LITTLEFS.h>
#include <RotaryEncoder.h>
#include <WebServer.h>
#include <WiFi.h>
#include "Arduino.h"
#include "debug.h"
#include "defaults.h"
#include "fanController.h"
#include "json.h"
#include "rotary.h"
#include "updateService.h"
#include "esp_log.h"
#include "json_handler.h"

// Variables
// ota URL
String otaUrl;
boolean otaUrlConfigured = false;

// Set in setup() if we fall to offline mode
boolean offlineMode = false;

// Wifi check
unsigned long previousCheckWifi = millis();

// Define custom update service.
UpdateService updateService;

// Define the adpter to handle hardware related io things.
Rotary rotary;

// Define the fans controller.
FanController fanController;

// Define webserver variable and tell autoconnect to use it
WebServer Server;
AutoConnect Portal(Server);

// Config Object
AutoConnectConfig WifiConfig;

// Custom Websites
AutoConnectCredential WifiCredentials;
AutoConnectAux otaSetting;
AutoConnectAux otaSave;

json_handler json_file;

// Webserver: serve a default page
void rootPage()
{
  char content[] = "Clean Air dummy site";
  Server.send(200, "text/plain", content);
} // rootPage

// Autoconnect: when AP is started print message and turn internal led on
bool startCP(IPAddress ip)
{
  digitalWrite(LED_BUILTIN, HIGH);
  ESP_LOGI(TAG, "CP started, IP: %s", WiFi.localIP().toString());
  return true;
} // startCP

// Get parameters from AutoConnectElement
void getParams()
{
  otaUrl = otaSetting.getElement<AutoConnectInput>(OTA_URL_KEY).value;
  otaUrlConfigured = true;
  otaUrl.trim();
  updateService.setServerUrl(otaUrl);
}

// Load parameter from eeprom fs
void loadParams(const char *paramFile)
{
  // Load the elements with parameters
  otaUrl = json_file.read_string_from_json(paramFile, OTA_URL_KEY);
  if (otaUrl != "")
  {
    otaSetting.setElementValue(OTA_URL_KEY, otaUrl);
    getParams();
    ESP_LOGI(TAG, "%s loaded", String(paramFile).c_str());
  }
  else
    ESP_LOGW(TAG, "%s failed to load", String(paramFile).c_str());
} // loadParams

// Save parameter to eeprom
void saveParams(const char *paramFile)
{
  otaUrl = otaSetting.getElement<AutoConnectInput>(OTA_URL_KEY).value;
  json_file.write_to_json(paramFile, OTA_URL_KEY, otaUrl);
} // saveParams

// Handler for custom webpage, needed to save data to eeprom
String onSave(AutoConnectAux &aux, PageArgument &args)
{
  getParams();
  saveParams(PARAM_FILE);
  return "Saved";
}

// Configures network related stuff during setup.
void configureNetwork()
{
  ESP_LOGD(TAG, "Configure Wifi: begin");

  // Configure Wifi Settings

  String hostName = "cleanair-" + String((uint32_t)ESP.getEfuseMac(), HEX);

  WifiConfig.hostName = hostName;
  WifiConfig.autoRise = false; // Disable CP portal. This is needed for "offline mode"
  WifiConfig.autoReconnect = true;
  WifiConfig.apid = AP_SSID;

  // Check if rotary button is pressed and start CP
  if (digitalRead(KY_SW) == LOW)
  {
    WifiConfig.immediateStart = true;
    WifiConfig.autoRise = true; // autorise must be true if we want to enter CP mode
    ESP_LOGI(TAG, "Rotary button pressed. Will start config AP and CP");
  }
  /*
  else if (WifiCredentials.entries() == 0) // Check configured Wifi Credential entries
  {
    ESP_LOGW(TAG, ""Found no wifi credentials. Assuming offline mode");
    offlineMode = true;
  }
  */
  ESP_LOGD(TAG, "WiFi connections: %d", WifiCredentials.entries());

  WifiConfig.autoReconnect = true;
  WifiConfig.apid = AP_SSID;
  Portal.config(WifiConfig);

  // detect captive portal
  Portal.onDetect(startCP);

  // Serve default webpage
  ESP_LOGD(TAG, "Serving webpage");
  Server.on("/", rootPage);

  // Load and set the custom configuration web page for ota updates
  otaSetting.load(otaSettingPage);
  loadParams(PARAM_FILE);
  otaSetting.menu(true);
  otaSave.load(otaSavePage);
  otaSave.on(onSave);  // custom handler when this site is called
  otaSave.menu(false); // hidden, only called from otaSettingPage

  Portal.config(WifiConfig);
  // Start the server
  Portal.join({otaSetting, otaSave});

  ESP_LOGD(TAG, "%d", WifiCredentials.entries());

  if (offlineMode == false) // if we already in offline mode skip portal startup
  {
    if (Portal.begin())
    {
      ESP_LOGI(TAG, "Webserver started: %s", WiFi.localIP().toString().c_str());
    }

    delay(1000);

    if (WiFi.status() != WL_CONNECTED && WifiConfig.autoRise == false)
    {
      ESP_LOGW(TAG, "Wifi not connected - offline mode");
      offlineMode = true;
    }
  }
  LITTLEFS.end();

  ESP_LOGD(TAG, "Configure Wifi: complete");
}

// TODO: no idea how to stuff the setspeed func directly into the confiure
void setFanSpeed(int speed)
{
  fanController.setSpeed(speed);
}

// Setup: Called once at bootup
void setup()
{
  json_file.init();

  esp_log_level_set("*", ESP_LOG_ERROR);
  esp_log_level_set(TAG, ESP_LOG_VERBOSE);

  Serial.begin(BAUD_RATE);

  ESP_LOGI(TAG, "Setup initiated");

  rotary.configure(setFanSpeed);
  fanController.configure();

  configureNetwork();

  ESP_LOGI(TAG, "Setup complete.");
  ESP_LOGD(TAG, "offlineMode: %d", offlineMode);
  ESP_LOGD(TAG, "otaUrlConfigured: %d", otaUrlConfigured);
  ESP_LOGD(TAG, "otaUrl: %s", otaUrl);

} // setup

// Loop: Main
void loop()
{
  // WIFI check, includes connection check and update check
  if (millis() - previousCheckWifi > CHECK_WIFI_INTERVAL)
  {
    if (WiFi.status() == WL_CONNECTED)
    {
      // Check if new a new update is available.
      updateService.checkAndUpdateSoftware();
    }
    previousCheckWifi = millis();
  }

  // Handle clients
  Portal.handleClient();

  // handlet the current rotary position
  rotary.handle();
} // loop
