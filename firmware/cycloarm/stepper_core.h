#ifndef STEPPER_CORE_H
#define STEPPER_CORE_H
#include "cycloarm.h"

// NOTE: I think the way for this to work is that the communication will all be done from
// the computer to one of the boards, i.e. board_a.
// if a motion segment is for board_b, then it will send that data
// over i2c where it will then be actioned.
// but there must be a way for the code to know which board it is on.
// will have to use compiler flags for this.

void stepper_init();
void move_segment(motion_segment_t motion);
void set_zero_position();
void calibrate();

position_response_t poll_positions();

extern motion_segment_t *last_command;

#ifdef BOARD_A
extern MultiStepper board_a_steppers;
constexpr uint32_t base_steps_per_rev = MOTOR_STEPS_PER_REV * BASE_RR;
extern uint32_t base_limit_low;
extern uint32_t base_limit_high;
constexpr uint32_t shoulder_steps_per_rev = MOTOR_STEPS_PER_REV * SHOULDER_RR;
extern uint32_t shoulder_limit_low;
extern uint32_t shoulder_limit_high;
constexpr uint32_t elbow_steps_per_rev = MOTOR_STEPS_PER_REV * ELBOW_RR;
extern uint32_t elbow_limit_low;
extern uint32_t elbow_limit_high;
#endif
#ifdef BOARD_B
extern MultiStepper board_b_steppers;
constexpr uint32_t wrist_roll_steps_per_rev = MOTOR_STEPS_PER_REV * WRIST_ROLL_RR;
extern uint32_t wrist_roll_limit_low;
extern uint32_t wrist_roll_limit_high;
constexpr uint32_t wrist_left_steps_per_rev = MOTOR_STEPS_PER_REV * WRIST_LEFT_RR;
extern uint32_t wrist_left_limit_low;
extern uint32_t wrist_left_limit_high;
constexpr uint32_t wrist_right_steps_per_rev = MOTOR_STEPS_PER_REV * WRIST_RIGHT_RR;
extern uint32_t wrist_right_limit_low;
extern uint32_t wrist_right_limit_high;
#endif

#endif