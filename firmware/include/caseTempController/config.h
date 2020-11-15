//*****************************************//
// configuration                           //
//*****************************************//
#define K_P                1        // proportional factor of the PID controller
#define K_I                0.1      // integral factor of the PID controller
#define K_D                0.5      // derivative factor of the PID controller
#define TAU                20       // number of measurements viewed in calculation, reduce to save space and calculation times
#define GOAL_TEMP          30       // goal temperature of 30°C
#define TEMP_GPIO          26       // the GPIO data pin of the temperature sensor
#define FAN_GPIO           23       // the PWM pin for fan control
#define FAN_PWM_CHANNEL    0        // the PWM channel for fan control
#define SLEEP              30000    // defines how many loop-throughs the controller will wait until measuring again