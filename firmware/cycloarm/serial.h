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

#define SERIAL_NO_DATA 0xff

void serial_init();

// Writes one byte of data to the serial buffer, as well as the type. Called by the main program.
// NOTE: The buffer structure is going to have to be:
// [...,type, data, type, data,...] since type and data are of known size we can read those bits specifically.
void serial_write(msg_type_t type, uint8_t data);

// Reads the first byte from the serial read buffer. Called from the main program.
// NOTE: May need to add message type somehow, or handle the different message types within this function
// Rather than returning the data itself.
uint8_t serial_read();

// Resets the data in the read buffer. Used when resetting.
void serial_reset_read_buffer();

// Returns the number of bytes in the rx buffer.
uint8_t serial_get_rx_buffer_count();

// Returns the number of bytes in the tx buffer.
uint8_t serial_get_tx_buffer_count();

#endif