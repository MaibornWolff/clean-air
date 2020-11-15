// Pin Mapping
#define FAN_PWM 33

// PWM configuration
#define FAN_FREQ 25000
#define FAN_RES 8

const int FAN_SPEED_1 = -1;
const int FAN_SPEED_2 = -1;
const int FAN_SPEED_3 = -1;
const int FAN_SPEED_4 = -1;

class FanController
{
public:
    // configures the knob to be able to interact with.
    void setup(/*void (*speedHandler)(int, int, int, int)*/);

    // Sets the fans speed according to the number handed in.
    void setSpeed(int);

    // Not implemented yet, but can later on be used to emit the fans speed.
    void emitFanSpeed();
};