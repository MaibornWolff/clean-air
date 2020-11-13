/**
 * Clean Air Rotary. Handles the logic related to the rotary.
 */
#include "defaults.h"
#include "jsonHandler.h"
#include "rotary.h"
#include <RotaryEncoder.h>

// Variables
// Rotary Encoder
RotaryEncoder encoder(KY_DT, KY_CLK);

// Last known rotary position.
int rotaryLastPos = -1;

// Define the JsonHandler.
extern JsonHandler jsonHandler;

// The callback function to handle changes emitted by the rotary
CallbackSpeedHandler cbSpeedHandler;

// Constructor.
void Rotary::setup(JsonHandler handler, CallbackSpeedHandler speedHandler)
{
    jsonHandler = handler;
    rotaryLastPos = jsonHandler.statistics[FAN_SPEED_KEY];
    if (!rotaryLastPos)
    {
        rotaryLastPos = ROTARYMAX;
    }

    cbSpeedHandler = speedHandler;
}

// Configure the rotary.
void Rotary::configure()
{
    ESP_LOGI(TAG, "Starting to configure rotary.");

    pinMode(LED_BUILTIN, OUTPUT);

    // Set Rotary default position
    ESP_LOGI(TAG, "Set rotary default position");
    encoder.setPosition(rotaryLastPos);
    cbSpeedHandler(rotaryLastPos);
}

// Handle the rotary element: Check the position and return the current position
int Rotary::calculateRotaryPosition()
{
    encoder.tick();

    // get the current physical position and calc the logical position
    int newPos = encoder.getPosition() * ROTARYSTEPS;

    if (newPos < ROTARYMIN)
    {
        encoder.setPosition(ROTARYMIN / ROTARYSTEPS);
        newPos = ROTARYMIN;
    }
    else if (newPos > ROTARYMAX)
    {
        encoder.setPosition(ROTARYMAX / ROTARYSTEPS);
        newPos = ROTARYMAX;
    }

    //Subtract from max value to increase val with cw rotation
    newPos = ROTARYMAX - newPos;

    return newPos;
} // calculateRotaryPosition

void updateRotaryPosition(int position)
{
    jsonHandler.statistics[FAN_SPEED_KEY] = position;
    jsonHandler.storeStatistics();
}

// Reacts on changes of knobs rotation.
void Rotary::handle()
{
    // Handle rotary element
    int rotaryCurPos = this->calculateRotaryPosition();
    if (rotaryLastPos != rotaryCurPos)
    {
        rotaryLastPos = rotaryCurPos;
        cbSpeedHandler(rotaryLastPos);
        updateRotaryPosition(rotaryLastPos);

        ESP_LOGV(TAG, "%d", rotaryCurPos);
    } // if
}