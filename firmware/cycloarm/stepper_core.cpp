#include "cycloarm.h"

void stepper_init()
{
#ifdef BOARD_A
    pinMode(BASE_STEP_PIN, OUTPUT);
    pinMode(BASE_DIR_PIN, OUTPUT);
    pinMode(SHOULDER_STEP_PIN, OUTPUT);
    pinMode(SHOULDER_DIR_PIN, OUTPUT);
    pinMode(ELBOW_STEP_PIN, OUTPUT);
    pinMode(ELBOW_DIR_PIN, OUTPUT);
#endif
#ifdef BOARD_B
    pinMode(WRIST_ROLL_STEP_PIN, OUTPUT);
    pinMode(WRIST_ROLL_DIR_PIN, OUTPUT);
    pinMode(WRIST_LEFT_STEP_PIN, OUTPUT);
    pinMode(WRIST_LEFT_DIR_PIN, OUTPUT);
    pinMode(WRIST_RIGHT_STEP_PIN, OUTPUT);
    pinMode(WRIST_RIGHT_DIR_PIN, OUTPUT);
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
#endif
#ifdef BOARD_B

    if (motion.board == BOARD_B_IDENT)
    {
    }
#endif
}