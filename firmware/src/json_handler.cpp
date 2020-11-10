#include <ArduinoJson.h>
#include <FS.h>
#include <LITTLEFS.h>
#include "json_handler.h"

json_handler::json_handler()
{
}

void json_handler::init()
{
    // Initialize LittleFS if not present
    if (!LITTLEFS.begin(FORMAT_LITTLEFS_IF_FAILED))
    {
        ESP_LOGE(TAG, "LITTLEFS Mount Failed");
        return;
    }
    LITTLEFS.end();
}

void json_handler::write_and_close(String filename)
{
    if (serializeJson(this->json_doc, this->file) == 0)
    {
        ESP_LOGE(TAG, "Failed to write file %s.", filename.c_str());
    }

    file.close();
    LITTLEFS.end();
}

void json_handler::open_file(String filename)
{
    LITTLEFS.begin();
    this->file = LITTLEFS.open(filename, FILE_WRITE);
    size_t size = file.size();

    if (size == 0)
    {
        ESP_LOGI(TAG, "File %s empty. Will create a new one...", filename.c_str());
    }
    else
    {
        DeserializationError error = deserializeJson(this->json_doc, this->file);
        if (error)
        {
            ESP_LOGI(TAG, "Failed to parse file %s. Will create a new one.", filename.c_str());
        }
    }
}

void json_handler::write_to_json(String filename, String key, String value)
{
    open_file(filename);
    json_doc[key] = value;
    write_and_close(filename);
}

void json_handler::write_to_json(String filename, String key, int value)
{
    open_file(filename);
    json_doc[key] = value;
    write_and_close(filename);
}

String json_handler::read_string_from_json(String filename, String key)
{
    String output = "";

    LITTLEFS.begin();
    File file = LITTLEFS.open(filename, FILE_READ);
    size_t size = file.size();

    if (size == 0)
    {
        ESP_LOGI(TAG, "File %s empty. Will create a new one...", filename.c_str());
    }
    else
    {
        StaticJsonDocument<DOC_SIZE> json_doc;
        DeserializationError error = deserializeJson(json_doc, file);
        if (error)
        {
            ESP_LOGI(TAG, "Failed to parse file %s. Will create a new one.", filename.c_str());
        }
        else
        {
            const char *out = json_doc[key];
            output = String(out);
        }
    }
    file.close();
    LITTLEFS.end();
    return output;
}
