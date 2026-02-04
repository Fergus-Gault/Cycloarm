#include "cycloarm.h"

void delay_ms(uint32_t ms)
{
    while (ms--)
    {
        _delay_ms(1);
    }
}

void delay_us(uint32_t us)
{
    while (us)
    {
        if (us < 10)
        {
            _delay_us(1);
            us--;
        }
        else if (us < 100)
        {
            _delay_us(10);
            us -= 10;
        }
        else if (us < 1000)
        {
            _delay_us(100);
            us -= 100;
        }
        else
        {
            _delay_ms(1);
            us -= 1000;
        }
    }
}