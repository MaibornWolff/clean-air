#include <ArduinoJson.h>
#include <FS.h>
#include <LITTLEFS.h>
#include "json_handler.h"

json_handler::json_handler()
{
}

void json_handler::dump_debug_info()
{
    ESP_LOGD(TAG, "Document size: %d", json_doc.size());
    String outstring;
    serializeJson(json_doc, outstring);
    ESP_LOGD(TAG, "Document string: %s", outstring.c_str());
}

void json_handler::dump_json(String filename)
{
    LITTLEFS.begin();
    ESP_LOGD(TAG, "Writing to file: %s", filename.c_str());
    dump_debug_info();
    file = LITTLEFS.open(filename, FILE_WRITE);
    if (serializeJson(json_doc, file) == 0)
    {
        ESP_LOGE(TAG, "Failed to write file %s.", filename.c_str());
    }
    file.close();
    LITTLEFS.end();
}

void json_handler::read_file(String filename)
{
    if (!LITTLEFS.begin(FORMAT_LITTLEFS_IF_FAILED))
    {
        ESP_LOGE(TAG, "LITTLEFS Mount Failed");
        return;
    }

    ESP_LOGD(TAG, "Opening file: %s", filename.c_str());
    file = LITTLEFS.open(filename, FILE_READ);
    size_t size = file.size();

    if (size == 0)
    {
        ESP_LOGI(TAG, "File %s empty. Will create a new one...", filename.c_str());
        json_doc.clear();
    }
    else
    {
        DeserializationError error = deserializeJson(json_doc, file);
        if (error)
        {
            ESP_LOGI(TAG, "Failed to parse file %s. Will create a new one.", filename.c_str());
            json_doc.clear();
        }
    }

    dump_debug_info();
    file.close();
    LITTLEFS.end();
}
