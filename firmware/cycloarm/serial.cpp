#include "cycloarm.h"

void serial_init()
{
    if (!Serial.available())
    {
        Serial.begin(BAUD_RATE);
    }
}

// This handles the movement of data between the computer and Arduino
// It will respond to requests if the data is provided
void serial_write(msg_type_t type, uint8_t data = NO_DATA)
{
    if (!Serial.available())
    {
        serial_init();
    }
    switch (type)
    {
    case MSG_PING:
        // This is simply pinging the device, it will respond with a MSG_PING_ACK
        Serial.write(MSG_PING);
        break;
    case MSG_PING_ACK:
        // Response when pinged
        Serial.write(MSG_PING_ACK);
        break;
    case MSG_STATUS:
        // Used for requesting or responding to a status request
        Serial.write(MSG_STATUS);
        if (data != NO_DATA)
        {
            Serial.write(data);
        }
        Serial.write(MSG_END);
        break;
    case MSG_SEGMENT:
        // Motion segment, requires data to follow
        // Will stop sending if no data provided, as serial_read() will wait until data is received
        if (data == NO_DATA)
        {
            break;
        }
        Serial.write(MSG_SEGMENT);
        Serial.write(data);
        break;
    case MSG_SEGMENT_ACK:
        // Simple response to ackowledge that data has been received.
        Serial.write(MSG_SEGMENT_ACK);
        break;
    case MSG_FAULT:
        // Will respond with diagnostics data if available
        Serial.write(MSG_FAULT);
        if (data != NO_DATA)
        {
            Serial.write(data);
        }
        Serial.write(MSG_END);
        break;
    case MSG_HOME:
        // Simple command sending
        Serial.write(MSG_HOME);
        break;
    case MSG_SET_ZERO:
        // Simple command sending
        Serial.write(MSG_SET_ZERO);
        break;
    case MSG_END:
        break;
    }
}

void serial_read()
{
    if (!Serial.available())
    {
        serial_init();
    }
    if (Serial.available() > 0)
    {
        serial_incoming_byte = Serial.read();
        switch (serial_incoming_byte)
        {
        case MSG_PING:
            serial_write(MSG_PING_ACK);
            break;
        case MSG_STATUS:
            // TODO: Create msg status function which will then write that data
            // status = get_status();
            // serial_write(MSG_STATUS, status);
            serial_peek_incoming_byte = Serial.peek();
            // Here we are looking at the next byte in the buffer to see if it is an MSG_END
            // If it isn't, then we know that the byte is actionable data, so we read it.
            if (serial_peek_incoming_byte != MSG_END)
            {
                serial_incoming_byte = Serial.read();
                // TODO: Action the incoming data
            }
            break;
        case MSG_SEGMENT:
            // TODO: Action that segment by loading it to a motion buffer or something
            // Since a motion segment, we know that data will follow, so we read it.
            serial_incoming_byte = Serial.read();
            // TODO: Action the incoming data
            // Then return an acknowledgement
            serial_write(MSG_SEGMENT_ACK);
            break;
        case MSG_FAULT:
            serial_peek_incoming_byte = Serial.peek();
            if (serial_peek_incoming_byte != MSG_END)
            {
                // Fault contains data, do something with it
                serial_incoming_byte = Serial.read();
                // handle_fault(serial_incoming_byte);
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
            // Nothing needs to happen, we just consume the byte
            break;
        }
    }
}