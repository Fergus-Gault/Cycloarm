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

// Will return an encoded mask of the current position of all the motors
// Or possibly a pointer to an array of positions
// Or the positions array could be a known location that this function just modifies.
int32_t poll_position();

#ifdef BOARD_A
AccelStepper base(AccelStepper::DRIVER, BASE_STEP_PIN, BASE_DIR_PIN);
AccelStepper shoulder(AccelStepper::DRIVER, SHOULDER_STEP_PIN, SHOULDER_DIR_PIN);
AccelStepper elbow(AccelStepper::DRIVER, ELBOW_STEP_PIN, ELBOW_DIR_PIN);
#endif
#ifdef BOARD_B
AccelStepper wrist_roll(AccelStepper::DRIVER, WRIST_ROLL_STEP_PIN, WRIST_ROLL_DIR_PIN);
AccelStepper wrist_left(AccelStepper::DRIVER, WRIST_LEFT_STEP_PIN, WRIST_LEFT_DIR_PIN);
AccelStepper wrist_right(AccelStepper::DRIVER, WRIST_RIGHT_STEP_PIN, WRIST_RIGHT_DIR_PIN);
#endif

#endif