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

// Will return an encoded mask of the current position of all the motors
// Or possibly a pointer to an array of positions
// Or the positions array could be a known location that this function just modifies.
int32_t poll_position();

#endif