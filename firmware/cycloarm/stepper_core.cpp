#include "cycloarm.h"

void stepper_init()
{
#ifdef BOARD_A
    base.setMaxSpeed(BASE_MAX_SPEED);
    base.setAcceleration(BASE_ACCEL);
    shoulder.setMaxSpeed(SHOULDER_MAX_SPEED);
    shoulder.setAcceleration(SHOULDER_ACCEL);
    elbow.setMaxSpeed(ELBOW_MAX_SPEED);
    elbow.setAcceleration(ELBOW_ACCEL);
#endif
#ifdef BOARD_B
    wrist_roll.setMaxSpeed(WRIST_ROLL_MAX_SPEED);
    wrist_roll.setAcceleration(WRIST_ROLL_ACCEL);
    wrist_left.setMaxSpeed(WRIST_LEFT_MAX_SPEED);
    wrist_left.setAcceleration(WRIST_LEFT_ACCEL);
    wrist_right.setMaxSpeed(WRIST_RIGHT_MAX_SPEED);
    wrist_right.setAcceleration(WRIST_RIGHT_ACCEL);
#endif
}

void move_segment(motion_segment_t motion)
{
#ifdef BOARD_A

    // If the movement is meant for board B, then forward the
    // message to board B.
    if (motion.board == BOARD_B_IDENT)
    {
        i2c_write(MSG_SEGMENT, motion);
        return;
    }
    base.moveTo(motion.position[0]);
    shoulder.moveTo(motion.position[1]);
    elbow.moveTo(motion.position[2]);
#endif
#ifdef BOARD_B

    if (motion.board == BOARD_B_IDENT)
    {
        wrist_roll.moveTo(motion.position[0]);
        wrist_left.moveTo(motion.position[1]);
        wrist_right.moveTo(motion.position[2]);
    }
#endif
}

void set_zero_position()
{
#ifdef BOARD_A
    base.setCurrentPosition(0);
    shoulder.setCurrentPosition(0);
    elbow.setCurrentPosition(0);
#endif
#ifdef BOARD_B
    wrist_roll.setCurrentPosition(0);
    wrist_left.setCurrentPosition(0);
    wrist_right.setCurrentPosition(0);
#endif
}

void calibrate()
{
}