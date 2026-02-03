/*
cpu_map_328p.h - This defines the pins and configuration specifically for
Atmega328p boards, like the Uno.
*/

#ifdef CA_PLATFORM
#error "CPU map already defined, CA_PLATFORM: " CA_PLATFORM
#endif

#define CA_PLATFORM "Atmega328p";

#define STEP_DDR DDRD
#define STEP_PORT PORTD
#define BASE_STEP_BIT 2                                                                     // Uno D2
#define SHOULDER_STEP_BIT 3                                                                 // Uno D3
#define ELBOW_STEP_BIT 4                                                                    // Uno D4
#define STEP_MASK ((1 << BASE_STEP_BIT) | (1 << SHOULDER_STEP_BIT) | (1 << ELBOW_STEP_BIT)) // All step bits encoded as a mask

#define DIRECTION_DDR DDRD
#define DIRECTION_PORT PORTD
#define BASE_DIR_BIT 5                                                                  // Uno D5
#define SHOULDER_DIR_BIT 6                                                              // Uno D6
#define ELBOW_DIR_BIT 7                                                                 // Uno D7
#define DIR_MASK ((1 << BASE_DIR_BIT) | (1 << SHOULDER_DIR_BIT) | (1 << ELBOW_DIR_BIT)) // All direction bit encoded as a mask