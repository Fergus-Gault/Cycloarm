#pragma once
#include <stdint.h>

struct StepperAxis
{
    // Pins
    uint8_t step_pin;
    uint8_t dir_pin;
    uint8_t en_pin;

    // State
    volatile int32_t pos_steps;
    int32_t target_steps;

    // Limits
    int32_t min_steps;
    int32_t max_steps;

    // Motion limits
    int32_t max_vel;   // steps per second
    int32_t max_accel; // steps per second squared

    // Runtime scheduling
    int32_t v_curr;
    uint32_t next_step_us;
    bool enabled;
};

void stepper_init(StepperAxis *axes, uint8_t num_joints, uint8_t en_pin);
void stepper_enable(bool enable);
void stepper_disable();

void stepper_set_limits(uint8_t joint, int32_t min_steps, int32_t max_steps);
void stepper_set_max_vel(uint8_t joint, int32_t steps_per_s);
void stepper_set_max_accel(uint8_t joint, int32_t steps_per_s2);

void stepper_set_target(uint8_t joint, int32_t target_steps);
void stepper_update(uint32_t current_time_us);

int32_t stepper_get_position(uint8_t joint);
bool stepper_at_target(uint8_t joint);