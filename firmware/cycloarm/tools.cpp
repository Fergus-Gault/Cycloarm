#include "cycloarm.h"

uint16_t rad_to_steps(uint16_t radians)
{
    return radians * (MOTOR_STEPS_PER_REV / (2 * PI));
}

uint16_t steps_to_rad(uint16_t steps)
{
    return steps * ((2 * PI) / MOTOR_STEPS_PER_REV);
}