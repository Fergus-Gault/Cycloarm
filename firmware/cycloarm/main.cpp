#include "cycloarm.h"

void setup()
{
    serial_init();
    i2c_init();
    stepper_init();
    delay(5000);
    calibrate();
}

void loop()
{
    // poll_commands();
    // stepper_update();
}