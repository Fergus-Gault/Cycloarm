#include "cycloarm.h"
#ifndef SERIAL_H
#define SERIAL_H

void serial_init();

void serial_write(msg_type_t type, bool respond, byte *data_pointer);

void serial_read();

void serial_send_structure(byte *struct_pointer, uint8_t struct_length);
void serial_read_structure(byte *struct_pointer, uint8_t struct_length);

extern uint8_t serial_incoming_byte;
extern uint8_t serial_peek_incoming_byte;

#endif