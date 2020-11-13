#include <ArduinoJson.h>
#include <FS.h>
#include <LITTLEFS.h>
#include "jsonHandler.h"

void JsonHandler::setup()
{
    loadParameter();
    loadStatistics();
}

void dump_debug_info(StaticJsonDocument<DOC_SIZE> &jsonDoc)
{
    ESP_LOGD(TAG, "Document size: %d", jsonDoc.size());

    String outstring;
    serializeJson(jsonDoc, outstring);

    ESP_LOGD(TAG, "Document string: %s", outstring.c_str());
}

void writeFile(String filename, StaticJsonDocument<DOC_SIZE> &jsonDoc)
{
    LITTLEFS.begin();

    ESP_LOGD(TAG, "Writing to file: %s", filename.c_str());
    dump_debug_info(jsonDoc);

    File file = LITTLEFS.open(filename, FILE_WRITE);
    if (serializeJson(jsonDoc, file) == 0)
    {
        ESP_LOGE(TAG, "Failed to write file %s.", filename.c_str());
    }
    file.close();

    LITTLEFS.end();
}

void readFile(String filename, StaticJsonDocument<DOC_SIZE> &jsonDoc)
{
    if (!LITTLEFS.begin(FORMAT_LITTLEFS_IF_FAILED))
    {
        ESP_LOGE(TAG, "LITTLEFS Mount Failed");
        return;
    }

    ESP_LOGD(TAG, "Opening file: %s", filename.c_str());

    File file = LITTLEFS.open(filename, FILE_READ);
    size_t size = file.size();

    if (size == 0)
    {
        ESP_LOGI(TAG, "File %s empty. Will create a new one...", filename.c_str());
        jsonDoc.clear();
    }
    else
    {
        DeserializationError error = deserializeJson(jsonDoc, file);
        if (error)
        {
            ESP_LOGI(TAG, "Failed to parse file %s. Will create a new one.", filename.c_str());
            jsonDoc.clear();
        }
    }

    dump_debug_info(jsonDoc);

    file.close();
    LITTLEFS.end();
}

void JsonHandler::loadStatistics()
{
    readFile(STATS_FILE, statistics);
}

void JsonHandler::loadParameter()
{
    readFile(PARAM_FILE, parameter);
}

void JsonHandler::storeStatistics()
{
    writeFile(STATS_FILE, statistics);
}

void JsonHandler::storeParameter()
{
    writeFile(PARAM_FILE, parameter);
}
