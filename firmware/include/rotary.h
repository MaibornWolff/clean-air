#include "jsonHandler.h"

// Configurations for the KY-040 rotary element

// Rotary configuration
#define ROTARYSTEPS 8
#define ROTARYMIN 0
#define ROTARYMAX 255
// ROTARYDEFAULT is multiplied with the ROTARYSTEPS
#define ROTARYDEFAULT 4

const int KY_CLK = 21;
const int KY_DT = 17;
const int KY_SW = 16;

// Callback defintion.
typedef void (*CallbackSpeedHandler)(int);

class Rotary
{
private:
    int calculateRotaryPosition();

public:
    // Constructor.
    void setup(JsonHandler, void (*)(int));

    // Reacts on changes of knobs rotation.
    void handle();
};