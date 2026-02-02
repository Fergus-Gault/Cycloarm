#include <Arduino.h>
#include "../common/stepper_core.h"
#include "pinmap.h"

StepperAxis axes[NUM_JOINTS];

void move_commands()
{
    if (!Serial.available())
        return;

    String line = Serial.readStringUntil('\n');
    line.trim();
    if (line.length() == 0)
        return;

    char axis_char;
    long steps;

    // Expect format: "X 100"
    int parsed = sscanf(line.c_str(), "%c %ld", &axis_char, &steps);
    if (parsed != 2)
    {
        Serial.println("Invalid command");
        return;
    }

    uint8_t axis;
    switch (axis_char)
    {
    case 'x':
        axis = 0;
        break;
    case 'y':
        axis = 1;
        break;
    case 'z':
        axis = 2;
        break;
    default:
        Serial.println("Unknown axis");
        return;
    }

    axes[axis].target_steps += steps;

    stepper_enable(true); // enable before motion

    Serial.print("Axis ");
    Serial.print(axis_char);
    Serial.print(" moving by ");
    Serial.println(steps);
}

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
    stepper_enable(false);
    Serial.println("Initialised motors on Board A");
}

void loop()
{
    move_commands();
    stepper_update(micros());
}