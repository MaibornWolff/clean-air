// Configurations for the KY-040 rotary element

// Rotary configuration
#define ROTARYSTEPS 2
#define ROTARYMIN 0
#define ROTARYMAX 16
// ROTARYDEFAULT is multiplied with the ROTARYSTEPS
#define ROTARYDEFAULT 4

const int KY_CLK = 21;
const int KY_DT = 17;
const int KY_SW = 16;

// Callback defintion.
typedef void (*CallbackSpeedHandler)(int);

class Rotary
{
public:
    // configures the knob to be able to interact with.
    void configure(void (*)(int));

    // Reacts on changes of knobs rotation.
    void handle();
};