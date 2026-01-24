#include <Arduino.h>
#include "arm.h"

Arm arm = Arm();

void setup()
{
    /* Initialize serial */
    Serial.begin(9600);
}

void loop()
{
    // Read serial and check for commands to move axis
    if (Serial.available() > 0)
    {
        String command = Serial.readStringUntil('\n');
        // Example command format: "MOVE X 100 1" (Move X axis 100 steps in direction 1)
        if (command.startsWith("MOVE"))
        {
            char axisChar;
            int steps, direction;
            sscanf(command.c_str(), "MOVE %c %d %d", &axisChar, &steps, &direction);
            int axis;
            switch (axisChar)
            {
            case 'X':
                axis = 0;
                break;
            case 'Y':
                axis = 1;
                break;
            case 'Z':
                axis = 2;
                break;
            case 'A':
                axis = 3;
                break;
            default:
                Serial.println("Invalid axis specified.");
                return;
            }
            arm.moveAxis(axis, steps, direction);
        }
    }
}
