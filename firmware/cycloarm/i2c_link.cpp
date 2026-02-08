#include "cycloarm.h"

// TODO: Update this to mimick the serial implementation.

void i2c_init()
{
    if (!Wire.available())
    {
        Wire.begin();
    }
}

template <typename T>
void i2c_write(msg_type_t type, T data)
{
    if (!Wire.available())
    {
        i2c_init();
    }
    switch (type)
    {
    case MSG_PING:
        Wire.write(MSG_PING);
        break;
    case MSG_PING_ACK:
        Wire.write(MSG_PING_ACK);
        break;
    case MSG_STATUS:
        Wire.write(MSG_STATUS);
        if (data != NO_DATA)
        {
            Wire.write(data);
        }
        Wire.write(MSG_END);
        break;
    case MSG_SEGMENT:
        if (data == NO_DATA)
        {
            break;
        }
        Wire.write(MSG_SEGMENT);
        Wire.write(data);
        break;
    case MSG_SEGMENT_ACK:
        Wire.write(MSG_SEGMENT_ACK);
        break;
    case MSG_FAULT:
        Wire.write(MSG_FAULT);
        if (data != NO_DATA)
        {
            Wire.write(data);
        }
        Wire.write(MSG_END);
        break;
    case MSG_HOME:
        Wire.write(MSG_HOME);
        break;
    case MSG_SET_ZERO:
        Wire.write(MSG_SET_ZERO);
        break;
    case MSG_END:
        break;
    }
}

void i2c_read()
{
    if (!Wire.available())
    {
        i2c_init();
    }
    if (Wire.available() > 0)
    {
        i2c_incoming_byte = Wire.read();
        switch (i2c_incoming_byte)
        {
        case MSG_PING:
            i2c_write(MSG_PING_ACK, NO_DATA);
            break;
        case MSG_STATUS:
            status_comb_t status = get_status();
            i2c_write(MSG_STATUS, status);
            i2c_peek_incoming_byte = Wire.peek();
            if (i2c_peek_incoming_byte != MSG_END)
            {
                i2c_incoming_byte = Wire.read();
                // TODO: Action incoming data
            }
            break;
        case MSG_SEGMENT:
            i2c_incoming_byte = Wire.read();
            // TODO: Action the incoming data
            i2c_write(MSG_SEGMENT_ACK, NO_DATA);
            break;
        case MSG_FAULT:
            i2c_peek_incoming_byte = Wire.peek();
            if (i2c_peek_incoming_byte != MSG_END)
            {
                i2c_incoming_byte = Wire.read();
                // handle_fault(i2c_incoming_byte);
                break;
            }
            // handle_fault();
            break;
        case MSG_HOME:
            // move_home();
            break;
        case MSG_SET_ZERO:
            // set_zero();
            break;
        case MSG_END:
            break;
        }
    }
}

void i2c_send_structure(byte *struct_pointer, uint8_t struct_length)
{
    Wire.write(struct_pointer, struct_length);
}

void i2c_read_structure(byte *struct_pointer, uint8_t struct_length)
{
    Wire.readBytes(struct_pointer, struct_length);
}