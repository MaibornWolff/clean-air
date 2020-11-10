#ifndef JSON_HANDLER_H_
#define JSON_HANDLER_H_

#include <ArduinoJson.h>

#define FORMAT_LITTLEFS_IF_FAILED true
#define DOC_SIZE 1024

class json_handler
{
private:
    File file;
    void dump_debug_info();

public:
    StaticJsonDocument<DOC_SIZE> json_doc;

    json_handler();
    void read_file(String filename);
    void dump_json(String filename);
};

#endif