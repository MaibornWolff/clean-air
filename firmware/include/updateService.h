#include "jsonHandler.h"

// The interval to check for updates in milliseconds. Currently set to 24h.
const int UPDATE_INTERVAL_IN_MS = 86400000;

// The filename which contains the list of versions.
const String FILENAME_LATEST = "LATEST";

const String FALLBACK_BASE_URL = "https://mzcleanairstorage01.blob.core.windows.net/cleanair-firmware/";

class UpdateService
{
public:
    // Constructor.
    void setup(JsonHandler);

    // Checks if the given interval has passed and then starts to check for new software.
    void checkAndUpdateSoftware();

    // Sets the updateserver url.
    void setServerUrl(String serverUrl);
};
