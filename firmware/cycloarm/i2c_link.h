#include "cycloarm.h"
#ifndef I2C_LINK_H
#define I2C_LINK_H

void i2c_init();

void i2c_write(msg_type_t type, bool respond, byte *data_pointer);

void i2c_read();

void i2c_send_structure(byte *struct_pointer, uint8_t struct_length);
void i2c_read_structure(byte *struct_pointer, uint8_t struct_length);

extern uint16_t i2c_incoming_byte;
extern uint16_t i2c_peek_incoming_byte;

#endif