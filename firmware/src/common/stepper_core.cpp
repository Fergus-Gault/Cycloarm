#include "stepper_core.h"
#include <Arduino.h>

static StepperAxis *g_axes = nullptr;
static uint8_t g_num_joints = 0;
static uint8_t g_en_pin = 0xFF;

void stepper_init(StepperAxis *axes, uint8_t num_joints, uint8_t en_pin)
{
    g_axes = axes;
    g_num_joints = num_joints;
    g_en_pin = en_pin;

    pinMode(g_en_pin, OUTPUT);

    for (uint8_t i = 0; i < num_joints; i++)
    {
        pinMode(axes[i].step_pin, OUTPUT);
        pinMode(axes[i].dir_pin, OUTPUT);
        axes[i].v_curr = 0;
        axes[i].next_step_us = 0;
        axes[i].enabled = false;
    }
}

void stepper_enable(bool enable)
{
    if (g_en_pin == 0xFF)
        return;

    digitalWrite(g_en_pin, enable ? LOW : HIGH);

    for (uint8_t i = 0; i < g_num_joints; i++)
    {
        g_axes[i].enabled = enable;
    }
}

void stepper_disable()
{
    stepper_enable(false);
}

void stepper_set_limits(uint8_t joint, int32_t min_steps, int32_t max_steps)
{
    // Code to set movement limits for a specific joint
}

void stepper_set_max_vel(uint8_t joint, int32_t steps_per_s)
{
    // Code to set maximum velocity for a specific joint
}

void stepper_set_max_accel(uint8_t joint, int32_t steps_per_s2)
{
    // Code to set maximum acceleration for a specific joint
}

void stepper_set_target(uint8_t joint, int32_t target_steps)
{
    // Code to set target position for a specific joint
}

void stepper_update(uint32_t current_time_us)
{
    // Code to update stepper motor positions
}

int32_t stepper_get_position(uint8_t joint)
{
    // Code to get current position of a specific joint
    return 0;
}

bool stepper_at_target(uint8_t joint)
{
    // Code to check if a specific joint has reached its target position
    return true;
}
