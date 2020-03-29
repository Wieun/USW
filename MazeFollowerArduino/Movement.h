#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <PID_v1.h>
#include "Arduino.h"
#include "Encoder.h"

class Movement{
private:
    #define ENABLE_LEFT 10
    #define ENABLE_RIGHT 9
    #define IN1 8
    #define IN2 7
    #define IN3 12
    #define IN4 11

    #define carSpeed   200
    #define leftSpeed  200
    #define rightSpeed 200

    int positionLeft = -999, positionRight = -999,
    newLeft, newRight;
public:
    void forward(int lSpeed, int rSpeed);
    void stop(int lSpeed, int rSpeed);
    void left(int lSpeed, int rSpeed);
    void right(int lSPeeed, int rSpeed);
    void setup();
};

#endif // MOVEMENT_H
