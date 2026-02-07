#include "cycloarm.h"
#ifndef SERIAL_H
#define SERIAL_H

void serial_init();

void serial_write(msg_type_t type, uint8_t data);

// All actioning called from within this function, so returns void.
void serial_read();

extern uint8_t serial_incoming_byte;
extern uint8_t serial_peek_incoming_byte;

#endif