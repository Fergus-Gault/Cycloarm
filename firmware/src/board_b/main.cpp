#include <Arduino.h>
#include "../common/stepper_core.h"
#include "pinmap.h"

StepperAxis axes[NUM_JOINTS];

void setup()
{
    Serial.begin(115200);
    for (uint8_t i = 0; i < NUM_JOINTS; i++)
    {
        axes[i].step_pin = STEP_PINS[i];
        axes[i].dir_pin = DIR_PINS[i];
        axes[i].en_pin = EN_PIN;
        axes[i].pos_steps = 0;
        axes[i].target_steps = 0;

        axes[i].min_steps = -10000;
        axes[i].max_steps = 10000;
        axes[i].max_vel = 1000;   // steps per second
        axes[i].max_accel = 5000; // steps per second squared
    }
    stepper_init(axes, NUM_JOINTS, EN_PIN);
    stepper_enable(true);
}

void loop()
{
    stepper_update(micros());
}