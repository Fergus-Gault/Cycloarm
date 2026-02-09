#ifndef PROTOCOL_H
#define PROTOCOL_H
#include "cycloarm.h"
typedef enum
{
    MSG_PING = 0x01,        // Ping the receiving device.
    MSG_PING_ACK = 0x02,    // Response sent when pinging
    MSG_STATUS = 0x03,      // Ask the receiving device for information.
    MSG_RESPOND = 0x04,     // Request that the message be responded to.
    MSG_SEGMENT = 0x10,     // Send a motion segment, further data will follow this message.
    MSG_SEGMENT_ACK = 0x11, // Acknowledgement that the motion segment has been received.
    MSG_FAULT = 0x20,       // Inform receiving device that there has been a fault.
    MSG_HOME = 0x22,        // Move to home position
    MSG_SET_ZERO = 0x23,    // Set the zero-point
    MSG_END = 0x30,         // End of message indicator
} msg_type_t;

typedef struct
{
    uint8_t board;
    uint32_t speed[4];
    uint32_t position[4];
} motion_segment_t;

typedef struct
{
    uint8_t board;
    uint32_t positions[4];
} position_response_t;

typedef struct
{
    uint8_t num_motors;
    motion_segment_t last_command;
} status_board_t;

typedef struct
{
    status_board_t board_a;
    status_board_t board_b;
} status_comb_t;

#endif