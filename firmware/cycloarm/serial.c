#include "cycloarm.h"

uint8_t serial_rx_buffer[RX_BUFFER_SIZE];
uint8_t serial_rx_buffer_head = 0;
// We set this to volatile as it will change frequently
volatile uint8_t serial_rx_buffer_tail = 0;

uint8_t serial_tx_buffer[TX_BUFFER_SIZE];
uint8_t serial_tx_buffer_head = 0;
volatile uint8_t serial_tx_buffer_tail = 0;

uint8_t serial_get_rx_buffer_count()
{
    // This prevents repeated calls to volatile
    uint8_t rxtail = serial_rx_buffer_tail;
    if (serial_rx_buffer_head >= rxtail)
    {
        return (serial_rx_buffer_head - rxtail);
    }
    return (RX_BUFFER_SIZE - (rxtail - serial_rx_buffer_head));
}

uint8_t serial_get_tx_buffer_count()
{
    // This prevents repeated calls to volatile
    uint8_t txtail = serial_tx_buffer_tail;
    if (serial_tx_buffer_head >= txtail)
    {
        return (serial_tx_buffer_head - txtail);
    }
    return (TX_BUFFER_SIZE - (txtail - serial_tx_buffer_head));
}

void serial_init()
{
    // Set baud rate
    uint16_t UBRR0_value = ((F_CPU / 4L * BAUD_RATE) - 1) / 2;
    UCSR0A |= (1 << U2X0);
    UBRR0H = UBRR0_value >> 8;
    UBRR0L = UBRR0_value;

    // Enable rx and tx
    UCSR0B |= 1 << RXEN0;
    UCSR0B |= 1 << TXEN0;

    // Enable interrupt on complete reception of a byte
    UCSR0B |= 1 << RXCIE0;
}

void serial_write(msg_type_t type, uint8_t data)
{
    uint8_t next_head = serial_tx_buffer_head + 1;
    if (next_head == TX_BUFFER_SIZE)
    {
        next_head = 0;
    }

    while (next_head == serial_tx_buffer_tail)
    {
        if (sys_rt_exec_state & EXEC_RESET)
        {
            return;
        }
    }
}