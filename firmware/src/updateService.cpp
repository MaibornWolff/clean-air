/**
 * Clean Air Update Service.
 */
#include <FS.h>
#include <HTTPClient.h>
#include <Update.h>
#include "updateService.h"
#include "defaults.h"
#include "jsonHandler.h"

struct HttpClientAndResult
{
    HTTPClient *httpClient;
    int statusCode;
};

// Variables
// The unit of time during which the last check for an update has taken place. Is dependent on the update interval.
u_long lastHourCheckedAt = 0;

// The header to fetch.
const char *headerKeys[] = {"location"};

// The storage accounts baseurl. Currently just something to test with and to have a backup url.
String base_url = FALLBACK_BASE_URL;

// The currently installed software version.
String currentVersion = "";

// Some httpClient.
HTTPClient httpClient;

// The Jsonfile containing the saved version
extern JsonHandler jsonHandler;

void UpdateService::setup(JsonHandler handler)
{
    jsonHandler = handler;
}

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

HttpClientAndResult ToFinalRedirection(String location)
{
    httpClient.setFollowRedirects(HTTPC_DISABLE_FOLLOW_REDIRECTS);
    httpClient.setReuse(false);
    httpClient.begin(location);
    httpClient.collectHeaders(headerKeys, 1);

    int statusCode = httpClient.GET();

    if (statusCode >= 300 && statusCode < 400)
    {
        if (httpClient.hasHeader("location"))
        {
            String newLocation = httpClient.header("location");
            httpClient.end();

            ESP_LOGI(TAG, "%s", newLocation.c_str());
            return ToFinalRedirection(newLocation);
        }
    }
    else
    {
        return {httpClient : &httpClient, statusCode : statusCode};
    }
}

// Requests the latest version from the server (blobstorage)
String getLatestVersionFromServer(String location)
{
    HttpClientAndResult result = ToFinalRedirection(location);

    String content = "";

    switch (result.statusCode)
    {
    case 200:
        content = (*result.httpClient).getString();
        if (content != "")
        {
            ESP_LOGI(TAG, "Successfully requested the latest software version. %s", content.c_str());
            break;
        }

        ESP_LOGW(TAG, "Received empty file from the blobstorage to check for the latest version.");
        break;
    case 404:
        ESP_LOGE(TAG, "Received statuscode 404. File may have been moved or deleted. Cannot check for latest software.");
        break;
    default:
        ESP_LOGE(TAG, "Could not check for latest software version due to unknown reason.\nI received the status code %d\nURL: %s%s", result.statusCode, base_url.c_str(), FILENAME_LATEST.c_str());
        break;
    }

    (*result.httpClient).end();
    return content;
}

// Downloads and "installs" the update.
void downloadAndUpdate(String version, String filename, const char *md5Hash)
{
    ESP_LOGI(TAG, "Starting to download new software version, downloading file with name: %s", filename);

    HttpClientAndResult result = ToFinalRedirection(base_url + filename);

    if (result.statusCode != 200)
    {
        ESP_LOGE(TAG, "Could not check for latest software version due to unknown reason.\n I received the status code %d\nURL: %s%s", result.statusCode, base_url.c_str(), filename.c_str());
        return;
    }

    if (md5Hash && !md5Hash[0])
    {
        Update.setMD5(md5Hash);
    }

    int contentLength = (*result.httpClient).getSize();
    if (!Update.begin(contentLength))
    {
        ESP_LOGE(TAG, "Could not update to latest software because there is not enough space available!");
        return;
    }

    WiFiClient *client = (*result.httpClient).getStreamPtr();
    size_t written = Update.writeStream(*client);
    ESP_LOGI(TAG, "OTA: %d/%d bytes written.\n", written, contentLength);

    if (written != contentLength)
    {
        ESP_LOGE(TAG, "Wrote partial binary. Giving up.");
        return;
    }

    if (!Update.end())
    {
        ESP_LOGE(TAG, "Error from Update.end(): %s", String(Update.getError()).c_str());
        return;
    }

    if (Update.isFinished())
    {
        ESP_LOGI(TAG, "Update successfully completed. Storing software version (%s), reboot afterwards.", version.c_str());

        jsonHandler.parameter[VERSION_KEY] = version;
        jsonHandler.storeParameter();

        // This line is specific to the ESP32 platform:
        ESP.restart();
    }
    else
    {
        ESP_LOGE(TAG, "Error from Update.isFinished(): %s", String(Update.getError()).c_str());
        return;
    }
}

// Check if the current version differs from the polled version.
void checkAndUpdate()
{
    String polledContent = getLatestVersionFromServer(base_url + FILENAME_LATEST);
    String polledVersion = extractLatestVersion(polledContent);

    if (polledVersion == "")
    {
        ESP_LOGW(TAG, "Could not fetch latest version from Server. Stopping Check.");
        return;
    }

    String polledFilename = extractFilename(polledContent);
    const char *expectedMD5Hash = extractHash(polledContent);

    // Set the sw version if the current version is not set.
    if (currentVersion == "")
    {
        String version = jsonHandler.parameter[VERSION_KEY].as<String>();

        ESP_LOGI(TAG, "Softwareversion from storage %s", version.c_str());
        currentVersion = version;
    }

    // Starts the actual update if the versions differ.
    if (currentVersion != polledVersion)
    {
        ESP_LOGI(TAG, "Softwareversion in devicestate %s", currentVersion.c_str());
        ESP_LOGI(TAG, "Softwareversion from blobstorage %s", polledVersion.c_str());
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
    u_long opHours = jsonHandler.statistics[OPERATING_HOURS_KEY].as<u_long>();

    if ((opHours % UPDATE_INTERVAL_IN_HOURS) == 0 && lastHourCheckedAt != opHours)
    {
        ESP_LOGI(TAG, "Update intervall has passed. Starting to check for updates. Currently running on software with version: %s", currentVersion.c_str());

        lastHourCheckedAt = opHours;
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
