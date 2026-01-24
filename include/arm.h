#ifndef ARM_H
#define ARM_H

class Arm
{
public:
    Arm();
    void calibrate();
    void moveAxis(int axis, int steps, int direction);

private:
    const int EN = 8;

    const int X_DIR = 5;
    const int X_STEP = 2;

    const int Y_DIR = 6;
    const int Y_STEP = 3;

    const int Z_DIR = 7;
    const int Z_STEP = 4;

    const int A_DIR = 13;
    const int A_STEP = 12;

    const int X_ENDSTOP = 9;
    const int Y_ENDSTOP = 10;
    const int Z_ENDSTOP = 11;
    const int ABORT = A0;
    const int HOLD = A1;
    const int RESUME = A2;
};

#endif