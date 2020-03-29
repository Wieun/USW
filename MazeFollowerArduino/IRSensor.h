#ifndef IRSENSOR_H
#define IRSENSOR_H

#include "Arduino.h"

class IRSensor{
    const int FRONT_IR = A1;
    const int RIGHT_IR = A0;
    const int LEFT_IR  = A2;
public:
    int leftSensorData();
    int rightSensorData();
    int frontSensorData();
    void setup();
};

#endif // IRSENSOR_H
