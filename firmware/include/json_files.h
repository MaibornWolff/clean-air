#ifndef JSON_FILES_H_
#define JSON_FILES_H_

#include <ArduinoJson.h>

#define FORMAT_LITTLEFS_IF_FAILED true
#define DOC_SIZE 1024

class json_file
{
private:
    StaticJsonDocument<DOC_SIZE> json_doc;
    File file;
    void write_and_close(String filename);
    void open_file(String filename);

public:
    json_file();
    void init();
    void write_to_json(String filename, String key, String value);
    void write_to_json(String filename, String key, int value);
    String read_string_from_json(String filename, String key);
};

#endif