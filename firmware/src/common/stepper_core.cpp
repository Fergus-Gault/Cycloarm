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
    digitalWrite(g_en_pin, HIGH);

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
    g_axes[joint].min_steps = min_steps;
    g_axes[joint].max_steps = max_steps;
}

void stepper_set_max_vel(uint8_t joint, int32_t steps_per_s)
{
    g_axes[joint].max_vel = steps_per_s;
}

void stepper_set_max_accel(uint8_t joint, int32_t steps_per_s2)
{
    g_axes[joint].max_accel = steps_per_s2;
}

void stepper_set_target(uint8_t joint, int32_t target_steps)
{
    g_axes[joint].target_steps = target_steps;
}

void stepper_update(uint32_t current_time_us)
{
    for (uint8_t i = 0; i < g_num_joints; i++)
    {
        StepperAxis &axis = g_axes[i];

        if (!axis.enabled)
            continue;

        if (axis.pos_steps == axis.target_steps)
        {
            axis.v_curr = 0;
            continue;
        }

        // Simple constant velocity movement towards target
        int32_t direction = (axis.target_steps > axis.pos_steps) ? 1 : -1;
        axis.v_curr = direction * axis.max_vel;

        // Time to next step
        if (current_time_us >= axis.next_step_us)
        {
            // Step
            digitalWrite(axis.dir_pin, (direction > 0) ? HIGH : LOW);
            digitalWrite(axis.step_pin, HIGH);
            delayMicroseconds(800); // Pulse width
            digitalWrite(axis.step_pin, LOW);
            delayMicroseconds(800);

            // Update position
            axis.pos_steps += direction;

            // Schedule next step
            axis.next_step_us = current_time_us + (1000000 / abs(axis.v_curr));
        }
    }
}

int32_t stepper_get_position(uint8_t joint)
{
    return g_axes[joint].pos_steps;
}

bool stepper_at_target(uint8_t joint)
{
    return g_axes[joint].pos_steps == g_axes[joint].target_steps;
}
