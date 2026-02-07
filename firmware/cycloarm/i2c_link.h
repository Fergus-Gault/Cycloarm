#include "cycloarm.h"
#ifndef I2C_LINK_H
#define I2C_LINK_H

void i2c_init();

template <typename T>
void i2c_write(msg_type_t type, T data);

void i2c_read();

extern uint8_t i2c_incoming_byte;
extern uint8_t i2c_peek_incoming_byte;

#endif