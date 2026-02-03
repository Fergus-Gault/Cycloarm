/*
 This file defines the messages and framing for communication over any transport method.
*/

#ifndef protocol_h
#define protocol_h
#include "cycloarm.h"
// Defining values so that devices know what the message type is
// The values themselves do not nessecarily matter.
// This is an agreement as to what the values mean.
typedef enum
{
    MSG_PING = 0x01,        // Diagnostics
    MSG_STATUS = 0x02,      // Diagnostics
    MSG_SEGMENT = 0x10,     // Motion
    MSG_SEGMENT_ACK = 0x11, // Motion
    MSG_FAULT = 0x20,       // Faults
    MSG_HOME = 0x22,        // Homing
    MSG_SET_ZERO = 0x23,    // Homing
} msg_type_t;

typedef struct __attribute__((packed))
{
    uint16_t segment_id;
    uint32_t duration_us;
    int32_t steps[3];
} motion_segment_t;

typedef struct __attribute__((packed))
{
    uint8_t state;
    uint8_t buffer_free;
    uint16_t last_fault;
} status_t;

#endif