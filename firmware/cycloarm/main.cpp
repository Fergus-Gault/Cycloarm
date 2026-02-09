#include "cycloarm.h"

void setup()
{
    serial_init();
    i2c_init();
    stepper_init();
    calibrate();
}

void loop()
{
    // poll_commands();
    // stepper_update();
}