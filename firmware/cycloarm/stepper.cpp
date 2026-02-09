#include "cycloarm.h"

#ifdef BOARD_A
MultiStepper board_a_steppers;
AccelStepper base(AccelStepper::DRIVER, BASE_STEP_PIN, BASE_DIR_PIN);
AccelStepper shoulder(AccelStepper::DRIVER, SHOULDER_STEP_PIN, SHOULDER_DIR_PIN);
AccelStepper elbow(AccelStepper::DRIVER, ELBOW_STEP_PIN, ELBOW_DIR_PIN);
uint32_t base_limit_low = 0;
uint32_t base_limit_high = base_steps_per_rev;
uint32_t shoulder_limit_low = 0;
uint32_t shoulder_limit_high = shoulder_steps_per_rev;
uint32_t elbow_limit_low = 0;
uint32_t elbow_limit_high = elbow_steps_per_rev;
#endif

#ifdef BOARD_B
MultiStepper board_b_steppers;
AccelStepper wrist_roll(AccelStepper::DRIVER, WRIST_ROLL_STEP_PIN, WRIST_ROLL_DIR_PIN);
AccelStepper wrist_left(AccelStepper::DRIVER, WRIST_LEFT_STEP_PIN, WRIST_LEFT_DIR_PIN);
AccelStepper wrist_right(AccelStepper::DRIVER, WRIST_RIGHT_STEP_PIN, WRIST_RIGHT_DIR_PIN);
AccelStepper gripper(AccelStepper::DRIVER, GRIPPER_STEP_PIN, GRIPPER_DIR_PIN);
uint32_t wrist_roll_limit_low = 0;
uint32_t wrist_roll_limit_high = wrist_roll_steps_per_rev;
uint32_t wrist_left_limit_low = 0;
uint32_t wrist_left_limit_high = wrist_left_steps_per_rev;
uint32_t wrist_right_limit_low = 0;
uint32_t wrist_right_limit_high = wrist_right_steps_per_rev;
uint32_t gripper_limit_low = 0;
uint32_t gripper_limit_high = gripper_steps_per_rev;
#endif

motion_segment_t *last_command;

void stepper_init()
{

// Set up motor parameters for board A motors
#ifdef BOARD_A
    pinMode(EN_PIN, OUTPUT);
    pinMode(SHOULDER_STEP_PIN, OUTPUT);
    pinMode(SHOULDER_DIR_PIN, OUTPUT);
    board_a_steppers.addStepper(base);
    board_a_steppers.addStepper(shoulder);
    board_a_steppers.addStepper(elbow);
    base.setMaxSpeed(BASE_MAX_SPEED);
    base.setAcceleration(BASE_ACCEL);
    shoulder.setMaxSpeed(SHOULDER_MAX_SPEED);
    shoulder.setAcceleration(SHOULDER_ACCEL);
    elbow.setMaxSpeed(ELBOW_MAX_SPEED);
    elbow.setAcceleration(ELBOW_ACCEL);
    // enable_steppers();
    print_debug("Initialised steppers on Board A.");
#endif

// Set up motor parameters for board B motors
#ifdef BOARD_B
    board_b_steppers.addStepper(wrist_roll);
    board_b_steppers.addStepper(wrist_left);
    board_b_steppers.addStepper(wrist_right);
    board_b_steppers.addStepper(gripper);
    wrist_roll.setMaxSpeed(WRIST_ROLL_MAX_SPEED);
    wrist_roll.setAcceleration(WRIST_ROLL_ACCEL);
    wrist_left.setMaxSpeed(WRIST_LEFT_MAX_SPEED);
    wrist_left.setAcceleration(WRIST_LEFT_ACCEL);
    wrist_right.setMaxSpeed(WRIST_RIGHT_MAX_SPEED);
    wrist_right.setAcceleration(WRIST_RIGHT_ACCEL);
    gripper.setMaxSpeed(GRIPPER_MAX_SPEED);
    gripper.setAcceleration(GRIPPER_ACCEL);
    enable_steppers();
    print_debug("Initialised steppers on Board B.");
#endif
}

void enable_steppers()
{
#ifdef BOARD_A
    base.setEnablePin(EN_PIN);
    base.enableOutputs();
    shoulder.setEnablePin(EN_PIN);
    base.enableOutputs();
    elbow.setEnablePin(EN_PIN);
    elbow.enableOutputs();
    print_debug("Enabled outputs on Board A.");
#endif

#ifdef BOARD_B
    wrist_roll.setEnablePin(EN_PIN);
    wrist_roll.enableOutputs();
    wrist_left.setEnablePin(EN_PIN);
    wrist_left.enableOutputs();
    wrist_right.setEnablePin(EN_PIN);
    wrist_right.enableOutputs();
    gripper.setEnablePin(EN_PIN);
    gripper.enableOutputs();
    print_debug("Enabled outputs on Board B.");
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
        i2c_write(MSG_SEGMENT, true, (byte *)&motion);
        return;
    }
    last_command = &motion;
    motion.position[0] = clamp(motion.position[0], base_limit_low, base_limit_high);
    motion.position[1] = clamp(motion.position[1], shoulder_limit_low, shoulder_limit_high);
    motion.position[2] = clamp(motion.position[2], elbow_limit_low, elbow_limit_high);
    board_a_steppers.moveTo((long *)motion.position);
    // This will run until all the steppers have reaches their position.
    // NOTE: This is blocking.
    print_debug("Moving steppers on board A.");
    while (board_a_steppers.run())
        continue;
#endif
#ifdef BOARD_B

    if (motion.board == BOARD_B_IDENT)
    {
        last_command = &motion;
        motion.position[0] = clamp(motion.position[0], wrist_roll_limit_low, wrist_roll_limit_high);
        motion.position[1] = clamp(motion.position[1], wrist_left_limit_low, wrist_left_limit_high);
        motion.position[2] = clamp(motion.position[2], wrist_right_limit_low, wrist_right_limit_high);
        motion.position[3] = clamp(motion.position[3], gripper_limit_low, gripper_limit_high);
        board_b_steppers.moveTo((long *)motion.position);
        print_debug("Moving steppers on board B.");
        while (board_b_steppers.run())
            continue;
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
    positions.positions[3] = gripper.currentPosition();
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
    gripper.setCurrentPosition(0);
#endif
}

void calibrate()
{
#ifdef BOARD_A
    print_debug("Starting calibration.");
    print_debug("Starting shoulder calibration.");
    calibrate_shoulder();
    print_debug("Shoulder calibration complete.");
    print_debug("Starting elbow calibration.");
    calibrate_elbow();
    print_debug("Elbow calibration complete.");
    print_debug("Calibration complete.");
#endif
}

void calibrate_shoulder()
{
#ifdef BOARD_A
    bool at_limit = false;
    shoulder.setSpeed(500);
    shoulder.move(1000);
    while (!at_limit)
    {
        shoulder.runSpeed();

        if (digitalRead(SHOULDER_LIMIT_PIN) == LOW)
        {
            print_debug("Detecting limit pin at LOW");
            at_limit = true;
            shoulder.stop();
        }
    }
    at_limit = false;
    print_debug("Detected end stop.");
    // shoulder.setCurrentPosition(0);

    // uint32_t off_switch = shoulder_steps_per_rev / 20;
    // shoulder.moveTo(off_switch);

    // while (shoulder.run())
    // {
    // }

    shoulder.move(1000);
    while (!at_limit)
    {
        shoulder.runSpeed();

        if (digitalRead(SHOULDER_LIMIT_PIN) == LOW)
        {
            at_limit = true;
            shoulder.stop();
        }
    }

    uint16_t current_pos = shoulder.currentPosition();
    uint16_t mid_point = current_pos / 2;
    shoulder_limit_high = current_pos;
    shoulder.moveTo(mid_point);

    while (shoulder.run())
    {
    }
    at_limit = false;
#endif
}

void calibrate_elbow()
{
#ifdef BOARD_A
    bool at_limit = false;
    elbow.move(5000);
    while (!at_limit)
    {
        while (elbow.run())
        {
            if (digitalRead(ELBOW_LIMIT_PIN) == LOW)
            {
                at_limit = true;
                elbow.stop();
                break;
            }
        }
    }
    print_debug("End stop detected.");
    at_limit = false;
    elbow.setCurrentPosition(0);

    // Move the motor off the switch
    uint16_t off_switch = elbow_steps_per_rev / 20;
    elbow.moveTo(off_switch);
    while (elbow.run())
    {
    }

    elbow.move(5000);
    while (!at_limit)
    {
        while (elbow.run())
        {
            if (digitalRead(ELBOW_LIMIT_PIN) == LOW)
            {
                at_limit = true;
                elbow.stop();
                break;
            }
        }
    }

    uint16_t current_pos = elbow.currentPosition();
    elbow_limit_high = current_pos;
    uint16_t mid_point = current_pos / 2;

    elbow.moveTo(mid_point);
    while (elbow.run())
    {
    }
    at_limit = false;
#endif
}

// Main stepper motor update.
// This will be called each tick in the main loop.
void stepper_update()
{
    if (motion_q_empty())
        return;
    motion_segment_t *current_seg = motion_dequeue();
    move_segment(*current_seg);
}