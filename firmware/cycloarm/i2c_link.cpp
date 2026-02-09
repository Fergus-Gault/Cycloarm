#include "cycloarm.h"

void i2c_init()
{
    if (!Wire.available())
    {
        Wire.begin();
    }
}

void i2c_write(msg_type_t type, bool respond, byte *data_pointer = NO_DATA)
{
    if (!Wire.available())
        i2c_init();

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

        if (*data_pointer != NO_DATA)
            i2c_send_structure(data_pointer, sizeof(&data_pointer));
        else
            Wire.write(MSG_RESPOND);
        Wire.write(MSG_END);
        break;
    case MSG_SEGMENT:
        Wire.write(MSG_SEGMENT);
        i2c_send_structure(data_pointer, sizeof(&data_pointer));
        if (respond)
            Wire.write(MSG_RESPOND);
        Wire.write(MSG_END);
        break;
    case MSG_SEGMENT_ACK:
        Wire.write(MSG_SEGMENT_ACK);
        break;
    case MSG_FAULT:
        Wire.write(MSG_FAULT);
        if (*data_pointer != NO_DATA)
            i2c_send_structure(data_pointer, sizeof(&data_pointer));
        if (respond)
            Wire.write(MSG_RESPOND);
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
    case MSG_RESPOND:
        break;
    }
}

void i2c_read()
{
    if (!Wire.available())
        i2c_init();

    if (Wire.available() > 0)
    {
        i2c_incoming_byte = Wire.read();
        switch (i2c_incoming_byte)
        {
        case MSG_PING:
            i2c_write(MSG_PING_ACK, false);
            break;
        case MSG_STATUS:
            i2c_peek_incoming_byte = Wire.peek();
            if (i2c_peek_incoming_byte == MSG_RESPOND)
            {
                Wire.read();
                status_comb_t status = get_status();
                i2c_write(MSG_STATUS, false, (byte *)&status);
            }
            i2c_peek_incoming_byte = Wire.peek();
            if (i2c_peek_incoming_byte != MSG_END)
            {
                i2c_incoming_byte = Wire.read();
                i2c_read_structure((byte *)&i2c_incoming_byte, sizeof(i2c_incoming_byte));
            }
        case MSG_SEGMENT:
            i2c_incoming_byte = Wire.read();
            i2c_read_structure((byte *)i2c_incoming_byte, sizeof(i2c_incoming_byte));
            motion_enqueue((motion_segment_t *)i2c_incoming_byte);
            i2c_peek_incoming_byte = Wire.peek();
            if (i2c_peek_incoming_byte == MSG_RESPOND)
                i2c_write(MSG_SEGMENT_ACK, false);
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
        case MSG_RESPOND:
            // This should never happen, as respond should be consumed within other cases.
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