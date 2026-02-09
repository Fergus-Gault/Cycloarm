/*
    cpu_map_328p.h - This defines the pins and configuration specifically for
    Atmega328p boards, like the Uno.
    We use compiler flags so that each board can be compiled with different values so they do not interfere.
*/

#ifdef CA_PLATFORM
#error "CPU map already defined, CA_PLATFORM: " CA_PLATFORM
#endif

#define CA_PLATFORM "Atmega328p";

#define EN_PIN 8

#ifdef BOARD_A
// Step pins for Board A
#define BASE_STEP_PIN 2     // Uno D2
#define SHOULDER_STEP_PIN 3 // Uno D3
#define ELBOW_STEP_PIN 4    // Uno D4
// Direction pins for Board A
#define BASE_DIR_PIN 5     // Uno D5
#define SHOULDER_DIR_PIN 6 // Uno D6
#define ELBOW_DIR_PIN 7    // Uno D7
#endif

#ifdef BOARD_B
// Step pins for Board B
#define WRIST_ROLL_STEP_PIN 2
#define WRIST_LEFT_STEP_PIN 3
#define WRIST_RIGHT_STEP_PIN 4
#define GRIPPER_STEP_PIN 12 // Auxillary step pin D12
// Direction pins for Board B
#define WRIST_ROLL_DIR_PIN 5
#define WRIST_LEFT_DIR_PIN 6
#define WRIST_RIGHT_DIR_PIN 7
#define GRIPPER_DIR_PIN 13 // Auxillary dir pin D13
#endif

// Define limit switches for both the shoulder and the elbow.
// Other axes will use soft limits, as the main concern is cables getting tangled
// rather than a hard limit being reached.
#define SHOULDER_LIMIT_PIN 10 // Uno D10
#define ELBOW_LIMIT_PIN 11    // Uno D11