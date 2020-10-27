const static char *otaSavePage PROGMEM = R"(
{
  "title": "OTA Setting",
  "uri": "/ota_save",
  "menu": false,
  "element": [
    {
      "name": "caption",
      "type": "ACText",
      "value": "OTA Save",
      "style": "text-align:center;color:#2f4f4f;padding:10px;"
    },
    {
      "name": "OK",
      "type": "ACSubmit",
      "value": "OK",
      "uri": "/"
    }
  ]
}
)";

// Configure additional websites via JSON
const static char *otaSettingPage PROGMEM = R"(
{
  "title": "OTA Setting",
  "uri": "/ota_setting",
  "menu": false,
  "element": [
    {
      "name": "header",
      "type": "ACText",
      "value": "Over-the-air update settings",
      "style": "text-align:center;color:#2f4f4f;padding:10px;"
    },
    {
      "name": "otaUrl",
      "type": "ACInput",
      "value": "",
      "label": "otaUrl",
      "placeholder": "http://<url of the update server>"
    },
    {
      "name": "save",
      "type": "ACSubmit",
      "value": "Save",
      "uri": "/ota_save"
    },
    {
      "name": "discard",
      "type": "ACSubmit",
      "value": "Discard",
      "uri": "/_ac"
    }
  ]
}
)";
