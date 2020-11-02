/**
 * Clean Air Update Service.
 */
#include <EEPROM.h>
#include <HTTPClient.h>
#include <Update.h>
#include <Preferences.h>
#include "updateService.h"

// Variables
// The unit of time during which the last check for an update has taken place. Is dependent on the update interval.
int lastCheckedAt = -1;

// The storage accounts baseurl. Currently just something to test with and to have a backup url.
String base_url = DEFAULT_BASE_URL;

// The currently installed software version.
String currentVersion = "";

// Some httpClient.
HTTPClient httpClient;

// NVS parameter storage
Preferences nvsData;

// Extracts the latest software version from given string.
String extractLatestVersion(String content)
{
    return content.substring(0, content.indexOf(';'));
}

// Exctracts the filename so the update can get downloaded
String extractFilename(String content)
{
    int startIndex = content.indexOf(';') + 1;
    int endIndex = content.indexOf(';', startIndex);
    if (endIndex == -1 || endIndex == 0)
    {
        endIndex = content.length() - 1;
    }

    return content.substring(startIndex, endIndex);
}

// Exctracts the md5hash from the file.
const char *extractHash(String content)
{
    int versionIndex = content.indexOf(';') + 1;
    int startIndex = content.indexOf(';', versionIndex);
    int endIndex = content.indexOf(';', startIndex);

    if (startIndex == -1 || startIndex == 0)
    {
        return NULL;
    }

    if (endIndex == -1 || endIndex == 0)
    {
        endIndex = content.length() - 1;
    }

    return (content.substring(startIndex, endIndex)).c_str();
}

// Requests the latest version from the server (blobstorage)
String getLatestVersionFromServer()
{
    httpClient.begin(base_url + FILENAME_LATEST);
    int statusCode = httpClient.GET();
    String content = "";

    switch (statusCode)
    {
    case 200:
        content = httpClient.getString();
        if (content != "")
        {
            ESP_LOGI(TAG, "Successfully requested the latest software version.");
            break;
        }

        ESP_LOGW(TAG, "Received empty file from the blobstorage to check for the latest version.");
        break;
    case 404:
        ESP_LOGE(TAG, "Received statuscode 404. File may have been moved or deleted. Cannot check for latest software.");
        break;
    default:
        ESP_LOGE(TAG, "Could not check for latest software version due to unknown reason.\nI received the status code %d\nURL: %s%s", statusCode, base_url, FILENAME_LATEST);
        break;
    }

    return content;
}

// Stores the current software Version to the eeprom.
void storeLatestVersion(const String &strToWrite)
{
    ESP_LOGI(TAG, "Storing software with version to nvs: %s", strToWrite);
    nvsData.begin("softwareVersion", false);
    nvsData.putString("softwareVersion", strToWrite);
    nvsData.end();
}

// Reads the store value from the eeprom
String getLatestVersionFromStorage()
{
    nvsData.begin("softwareVersion", true);
    String nvsVersion = nvsData.getString("softwareVersion", "undef");
    nvsData.end();
    return nvsVersion;
}

// Downloads and "installs" the update.
void downloadAndUpdate(String version, String filename, const char *md5Hash)
{
    ESP_LOGI(TAG, "Starting to download new software version, downloading file with name: %s", filename);

    httpClient.begin(base_url + filename);
    int statusCode = httpClient.GET();

    if (statusCode != 200)
    {
        ESP_LOGE(TAG, "Could not check for latest software version due to unknown reason.\n I received the status code %d\nURL: %s%s", statusCode, base_url, filename);
        return;
    }

    if (md5Hash && !md5Hash[0])
    {
        Update.setMD5(md5Hash);
    }

    int contentLength = httpClient.getSize();
    if (!Update.begin(contentLength))
    {
        ESP_LOGE(TAG, "Could not update to latest software because there is not enough space available!");
        return;
    }

    WiFiClient *client = httpClient.getStreamPtr();
    size_t written = Update.writeStream(*client);
    ESP_LOGI(TAG, "OTA: %d/%d bytes written.\n", written, contentLength);
    if (written != contentLength)
    {
        ESP_LOGE(TAG, "Wrote partial binary. Giving up.");
        return;
    }

    if (!Update.end())
    {
        ESP_LOGE(TAG, "Error from Update.end(): %s", String(Update.getError()));
        return;
    }

    if (Update.isFinished())
    {
        ESP_LOGI(TAG, "Update successfully completed. Storing new version to e2prom and rebooting");
        storeLatestVersion(version);

        // This line is specific to the ESP32 platform:
        ESP.restart();
    }
    else
    {
        ESP_LOGE(TAG, "Error from Update.isFinished(): %s", String(Update.getError()));
        return;
    }
}

// Check if the current version differs from the polled version.
void checkAndUpdate()
{
    String polledContent = getLatestVersionFromServer();
    String polledVersion = extractLatestVersion(polledContent);
    String polledFilename = extractFilename(polledContent);
    const char *expectedMD5Hash = extractHash(polledContent);

    // Set the sw version if the current version is not set.
    if (currentVersion == "")
    {
        currentVersion = getLatestVersionFromStorage();
    }
    // Starts the actual update if the versions differ.
    if (currentVersion != polledVersion)
    {
        downloadAndUpdate(polledVersion, polledFilename, expectedMD5Hash);
    }
    else
    {
        ESP_LOGI(TAG, "Latest software already installed: %s", currentVersion);
    }
}

// Checks if the given interval has passed and then starts to check for new software.
void UpdateService::checkAndUpdateSoftware()
{
    // The # units since the device has started.
    int upTime = round(millis() / UPDATE_INTERVAL_IN_MS);

    if (upTime != lastCheckedAt)
    {
        ESP_LOGI(TAG, "A day has passed. Starting to check for updates. Currently running on software with version: %s", currentVersion);

        lastCheckedAt = upTime;
        checkAndUpdate();
    }
}

// Sets the updateserver url.
void UpdateService::setServerUrl(String serverUrl)
{
    if (serverUrl.length() > 0)
    {
        base_url = serverUrl;
    }
}
