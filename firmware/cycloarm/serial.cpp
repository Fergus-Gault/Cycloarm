#include "cycloarm.h"

void serial_init()
{
    if (!Serial.available())
        Serial.begin(BAUD_RATE);
    print_string("Serial initialised.");
}

// This handles the movement of data between the computer and Arduino
// It will respond to requests if the data is provided
void serial_write(msg_type_t type, bool respond, byte *data_pointer = NO_DATA)
{
    if (!Serial.available())
        serial_init();

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
        // Only send the data if we have data to send
        if (*data_pointer != NO_DATA)
            serial_send_structure(data_pointer, sizeof(&data_pointer));
        else
            Serial.write(MSG_RESPOND);
        Serial.write(MSG_END);
        break;
    case MSG_SEGMENT:
        Serial.write(MSG_SEGMENT);
        serial_send_structure(data_pointer, sizeof(&data_pointer));
        // Tell the receiver if we want a response for confirmation.
        if (respond)
            Serial.write(MSG_RESPOND);
        Serial.write(MSG_END);
        break;
    case MSG_SEGMENT_ACK:
        // Simple response to ackowledge that data has been received.
        Serial.write(MSG_SEGMENT_ACK);
        break;
    case MSG_FAULT:
        // Will respond with diagnostics data if available
        Serial.write(MSG_FAULT);
        if (*data_pointer != NO_DATA)
            serial_send_structure(data_pointer, sizeof(&data_pointer));
        if (respond)
            Serial.write(MSG_RESPOND);
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
    case MSG_RESPOND:
        break;
    }
}

void serial_read()
{
    if (!Serial.available())
        serial_init();

    if (Serial.available() > 0)
    {
        serial_incoming_byte = Serial.read();
        switch (serial_incoming_byte)
        {
        case MSG_PING:
            serial_write(MSG_PING_ACK, false);
            print_string("Received ping over serial.");
            break;
        case MSG_STATUS:
            serial_peek_incoming_byte = Serial.peek();
            print_string("Received status request over serial.");
            if (serial_peek_incoming_byte == MSG_RESPOND)
            {
                // Consume data
                Serial.read();
                status_comb_t status = get_status();
                // Send back the status
                serial_write(MSG_STATUS, false, (byte *)&status);
            }
            serial_peek_incoming_byte = Serial.peek();
            if (serial_peek_incoming_byte != MSG_END)
            {
                serial_incoming_byte = Serial.read();
                serial_read_structure((byte *)&serial_incoming_byte, sizeof(serial_incoming_byte));
            }
            break;
        case MSG_SEGMENT:
            // TODO: Load and iterate over the data received and when it is complete, send the movement
            // struct to the stepper system. I think this will require a motion buffer
            // to queue motions.
            // TODO: Action that segment by loading it to a motion buffer or something
            // Since a motion segment, we know that data will follow, so we read it.
            print_string("Received motion segment over serial.");
            serial_incoming_byte = Serial.read();
            serial_read_structure((byte *)serial_incoming_byte, sizeof(serial_incoming_byte));
            motion_enqueue((motion_segment_t *)serial_incoming_byte);
            // Then return an acknowledgement
            serial_peek_incoming_byte = Serial.peek();
            if (serial_peek_incoming_byte == MSG_RESPOND)
                serial_write(MSG_SEGMENT_ACK, false);
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
        case MSG_RESPOND:
            // This should never happen, as respond should be consumed within other cases.
            break;
        }
    }
}

void serial_send_structure(byte *struct_pointer, uint8_t struct_length)
{
    Serial.write(struct_pointer, struct_length);
}

void serial_read_structure(byte *struct_pointer, uint8_t struct_length)
{
    Serial.readBytes(struct_pointer, struct_length);
}