/*
    cpu_map_328p.h - This defines the pins and configuration specifically for
    Atmega328p boards, like the Uno.
    We use things like DDRD and PORTD as these are the port registers, this allows for faster
    manipulation of the I/O ports.
    We also use compiler flags so that each board can be compiled with different values so they do not interfere.
*/

#ifdef CA_PLATFORM
#error "CPU map already defined, CA_PLATFORM: " CA_PLATFORM
#endif

#define CA_PLATFORM "Atmega328p";

#define SERIAL_RX USART_RX_vect
#define SERIAL_URDE USART_URDE_vect

// Below uses PORTD, this maps to the Arduino digital pins 0-7.
// DDRD - Port D Data Direction Register - read/write
// PORTD - The Port D Data Register - read/write

// Step pins definition
#define STEP_DDR DDRD
#define STEP_PORT PORTD

#ifdef BOARD_A
// Step pins for Board A
#define BASE_STEP_BIT 2                                                                       // Uno D2
#define SHOULDER_STEP_BIT 3                                                                   // Uno D3
#define ELBOW_STEP_BIT 4                                                                      // Uno D4
#define STEP_MASK_A ((1 << BASE_STEP_BIT) | (1 << SHOULDER_STEP_BIT) | (1 << ELBOW_STEP_BIT)) // All step bits encoded as a mask
#endif
#ifdef BOARD_B
// Step pins for Board B
#define WRIST_ROLL_STEP_BIT 2
#define WRIST_LEFT_STEP_BIT 3
#define WRIST_RIGHT_STEP_BIT 4
#define STEP_MASK_B ((1 << WRIST_ROLL_STEP_BIT) | (1 << WRIST_LEFT_STEP_BIT) | (1 << WRIST_RIGHT_STEP_BIT))
#endif

// Direction pins definition
#define DIRECTION_DDR DDRD
#define DIRECTION_PORT PORTD

#ifdef BOARD_A
// Direction pins for Board A
#define BASE_DIR_BIT 5                                                                    // Uno D5
#define SHOULDER_DIR_BIT 6                                                                // Uno D6
#define ELBOW_DIR_BIT 7                                                                   // Uno D7
#define DIR_MASK_A ((1 << BASE_DIR_BIT) | (1 << SHOULDER_DIR_BIT) | (1 << ELBOW_DIR_BIT)) // All direction bit encoded as a mask
#endif
#ifdef BOARD_B
// Direction pins for Board B
#define WRIST_ROLL_DIR_BIT 5
#define WRIST_LEFT_DIR_BIT 6
#define WRIST_RIGHT_DIR_BIT 7
#define DIR_MASK_B ((1 << WRIST_ROLL_DIR_BIT) | (1 << WRIST_LEFT_DIR_BIT) | (1 << WRIST_RIGHT_DIR_BIT))
#endif

// Define limit switches for both the shoulder and the elbow.
// Other axes will use soft limits, as the main concern is cables getting tangled
// rather than a hard limit being reached.
// PORTB maps to Arduino digital pins 8-13
// DDRB - Port B Data Direction Register - read/write
// PORTB - Port B Data Register - read/write
// PINB - Port B Input Pin Register - read only
#define LIMIT_DDR DDRB
#define LIMIT_PIN PINB
#define LIMIT_PORT PORTB
#define SHOULDER_LIMIT_BIT 2                                            // Uno D10 (Second bit of PORTB)
#define ELBOW_LIMIT_BIT 3                                               // Uno D11
#define LIMIT_MASK ((1 << SHOULDER_LIMIT_BIT) | (1 << ELBOW_LIMIT_BIT)) // All limit bits encoded as a mask