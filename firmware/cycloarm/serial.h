/*
This defines the low level functions for sending a receiving bytes
via the serial port from a computer.
*/

#include "cycloarm.h"

#ifndef serial_h
#define serial_h

// Defining the buffer sizes for the RX (receiving) and TX (transmitting) port
#ifndef RX_BUFFER_SIZE
#define RX_BUFFER_SIZE 128
#endif
#ifndef TX_BUFFER_SIZE
#define TX_BUFFER_SIZE 64
#endif

#endif