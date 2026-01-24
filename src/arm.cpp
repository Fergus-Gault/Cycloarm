#include <Arduino.h>
#include "arm.h"

Arm::Arm()
{

    /* Configure the steper drive pins as outputs */
    pinMode(EN, OUTPUT);
    pinMode(X_DIR, OUTPUT);
    pinMode(X_STEP, OUTPUT);
    pinMode(Y_DIR, OUTPUT);
    pinMode(Y_STEP, OUTPUT);
    pinMode(Z_DIR, OUTPUT);
    pinMode(Z_STEP, OUTPUT);
    pinMode(A_DIR, OUTPUT);
    pinMode(A_STEP, OUTPUT);

    /* Configure the control pins as inputs with pullups */
    pinMode(X_ENDSTOP, INPUT_PULLUP);
    pinMode(Y_ENDSTOP, INPUT_PULLUP);
    pinMode(Z_ENDSTOP, INPUT_PULLUP);

    pinMode(ABORT, INPUT_PULLUP);
    pinMode(HOLD, INPUT_PULLUP);
    pinMode(RESUME, INPUT_PULLUP);

    /* Enable the X, Y, Z & Aux stepper outputs */
    digitalWrite(EN, LOW); // Low to enable
}

void Arm::calibrate()
{
    Serial.println("Calibrating...");

    // Move X axis towards endstop
    digitalWrite(X_DIR, LOW);              // Set direction towards endstop
    while (digitalRead(X_ENDSTOP) == HIGH) // While endstop not triggered
    {
        digitalWrite(X_STEP, HIGH);
        delayMicroseconds(800);
        digitalWrite(X_STEP, LOW);
        delayMicroseconds(800);
    }
    Serial.println("X axis calibrated.");

    // Move Y axis towards endstop
    digitalWrite(Y_DIR, LOW);              // Set direction towards endstop
    while (digitalRead(Y_ENDSTOP) == HIGH) // While endstop not triggered
    {
        digitalWrite(Y_STEP, HIGH);
        delayMicroseconds(800);
        digitalWrite(Y_STEP, LOW);
        delayMicroseconds(800);
    }
    Serial.println("Y axis calibrated.");

    // Move Z axis towards endstop
    digitalWrite(Z_DIR, LOW);              // Set direction towards endstop
    while (digitalRead(Z_ENDSTOP) == HIGH) // While endstop not triggered
    {
        digitalWrite(Z_STEP, HIGH);
        delayMicroseconds(800);
        digitalWrite(Z_STEP, LOW);
        delayMicroseconds(800);
    }
    Serial.println("Z axis calibrated.");

    Serial.println("Calibration complete.");
}

void Arm::moveAxis(int axis, int steps, int direction)
{
    int dirPin, stepPin, endStop;

    // Determine which pins to use based on the axis
    switch (axis)
    {
    case 0: // X axis
        dirPin = X_DIR;
        stepPin = X_STEP;
        endStop = X_ENDSTOP;
        break;
    case 1: // Y axis
        dirPin = Y_DIR;
        stepPin = Y_STEP;
        endStop = Y_ENDSTOP;
        break;
    case 2: // Z axis
        dirPin = Z_DIR;
        stepPin = Z_STEP;
        endStop = Z_ENDSTOP;
        break;
    case 3: // A axis
        dirPin = A_DIR;
        stepPin = A_STEP;
        endStop = -1; // No endstop for A axisw
        break;
    default:
        Serial.println("Invalid axis specified.");
        return;
    }

    // Set the direction
    if (direction == 0)
        digitalWrite(dirPin, LOW); // e.g., LOW for one direction
    else
        digitalWrite(dirPin, HIGH); // e.g., HIGH for the other direction

    // Perform the steps
    for (int i = 0; i < steps; i++)
    {
        digitalWrite(stepPin, HIGH);
        delayMicroseconds(800); // Step pulse width
        digitalWrite(stepPin, LOW);
        delayMicroseconds(800); // Step interval
        // If an endstop is defined, check it
        if (endStop != -1 && digitalRead(endStop) == LOW)
        {
            Serial.println("Endstop triggered, stopping movement.");
            break;
        }
    }

    Serial.print("Moved axis ");
    Serial.print(axis);
    Serial.print(" by ");
    Serial.print(steps);
    Serial.println(" steps.");
}