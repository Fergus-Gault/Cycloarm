#include "cycloarm.h"

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
void stepper_init()
{

// Set up motor parameters for board A motors
#ifdef BOARD_A
    board_a_steppers.addStepper(base);
    board_a_steppers.addStepper(shoulder);
    board_a_steppers.addStepper(elbow);
    base.setMaxSpeed(BASE_MAX_SPEED);
    base.setAcceleration(BASE_ACCEL);
    shoulder.setMaxSpeed(SHOULDER_MAX_SPEED);
    shoulder.setAcceleration(SHOULDER_ACCEL);
    elbow.setMaxSpeed(ELBOW_MAX_SPEED);
    elbow.setAcceleration(ELBOW_ACCEL);
    base_limit_low = 0;
    base_limit_high = base_steps_per_rev;
    shoulder_limit_low = 0;
    shoulder_limit_high = shoulder_steps_per_rev;
    elbow_limit_low = 0;
    elbow_limit_high = elbow_steps_per_rev;

#endif

// Set up motor parameters for board B motors
#ifdef BOARD_B
    board_b_steppers.addStepper(wrist_roll);
    board_b_steppers.addStepper(wrist_left);
    board_b_steppers.addStepper(wrist_right);
    wrist_roll.setMaxSpeed(WRIST_ROLL_MAX_SPEED);
    wrist_roll.setAcceleration(WRIST_ROLL_ACCEL);
    wrist_left.setMaxSpeed(WRIST_LEFT_MAX_SPEED);
    wrist_left.setAcceleration(WRIST_LEFT_ACCEL);
    wrist_right.setMaxSpeed(WRIST_RIGHT_MAX_SPEED);
    wrist_right.setAcceleration(WRIST_RIGHT_ACCEL);
    wrist_roll_limit_low = 0;
    wrist_roll_limit_high = wrist_roll_steps_per_rev;
    wrist_left_limit_low = 0;
    wrist_left_limit_high = wrist_left_steps_per_rev;
    wrist_right_limit_low = 0;
    wrist_right_limit_high = wrist_right_steps_per_rev;
#endif
}

// This movement function checks that the positions requested are within the limits
// and also which board the motion is requested on.
// All motion commands will be sent to board A, and then forwarded to board B
// if required. This means that there only needs to be one connection to the computer.
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
    last_command = &motion;
    motion.position[0] = clamp(motion.position[0], base_limit_low, base_limit_high);
    motion.position[1] = clamp(motion.position[1], shoulder_limit_low, shoulder_limit_high);
    motion.position[2] = clamp(motion.position[2], elbow_limit_low, elbow_limit_high);
    board_a_steppers.moveTo((long *)motion.position);
    board_a_steppers.run();
#endif
#ifdef BOARD_B

    if (motion.board == BOARD_B_IDENT)
    {
        last_command = &motion;
        motion.position[0] = clamp(motion.position[0], wrist_roll_limit_low, wrist_roll_limit_high);
        motion.position[1] = clamp(motion.position[1], wrist_left_limit_low, wrist_left_limit_high);
        motion.position[2] = clamp(motion.position[2], wrist_right_limit_low, wrist_right_limit_high);

        board_b_steppers.moveTo((long *)motion.position);
        board_b_steppers.run();
    }
#endif
}

position_response_t poll_positions()
{
    position_response_t positions;
#ifdef BOARD_A
    positions.board = BOARD_A_IDENT;
    positions.positions[0] = base.currentPosition();
    positions.positions[1] = shoulder.currentPosition();
    positions.positions[2] = elbow.currentPosition();
    return positions;
#endif
#ifdef BOARD_B
    positions.board = BOARD_B_IDENT;
    positions.positions[0] = wrist_roll.currentPosition();
    positions.positions[1] = wrist_left.currentPosition();
    positions.positions[2] = wrist_right.currentPosition();
    return positions;

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
#ifdef BOARD_A
    // Calibrate shoulder motor
    while (digitalRead(SHOULDER_LIMIT_PIN) == LOW)
    {
        shoulder.move(100);
        shoulder.run();
    }
    shoulder.stop();
    shoulder.setCurrentPosition(0);
    uint32_t off_switch = shoulder_steps_per_rev / 20;
    while ((uint32_t)shoulder.currentPosition() != off_switch)
    {
        shoulder.moveTo(off_switch);
        shoulder.run();
    }
    while (digitalRead(SHOULDER_LIMIT_PIN) == LOW)
    {
        shoulder.move(-100);
        shoulder.run();
    }
    shoulder.stop();
    uint16_t current_pos = shoulder.currentPosition();
    shoulder_limit_high = current_pos;
    uint16_t mid_point = current_pos / 2;
    shoulder.moveTo(mid_point);
    while (!shoulder.distanceToGo() != 0)
    {
        shoulder.run();
    }

    // Calibrate elbow motor
    while (digitalRead(ELBOW_LIMIT_PIN) == LOW)
    {
        elbow.move(100);
        elbow.run();
    }
    elbow.stop();
    elbow.setCurrentPosition(0);
    // Move the motor off the switch
    off_switch = elbow_steps_per_rev / 20;
    while ((uint32_t)elbow.currentPosition() != off_switch)
    {
        elbow.moveTo(off_switch);
        elbow.run();
    }
    while (digitalRead(ELBOW_LIMIT_PIN) == LOW)
    {
        elbow.move(-100);
        elbow.run();
    }
    elbow.stop();
    current_pos = elbow.currentPosition();
    elbow_limit_high = current_pos;
    mid_point = current_pos / 2;
    elbow.moveTo(mid_point);
    while (!elbow.distanceToGo() != 0)
    {
        elbow.run();
    }
#endif
}