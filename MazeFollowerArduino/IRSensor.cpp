#include "IRSensor.h"

void IRSensor::setup(){
    pinMode(FRONT_IR, INPUT);
    pinMode(RIGHT_IR, INPUT);
    pinMode(LEFT_IR , INPUT);
    Serial.begin(9600);
}

int IRSensor::leftSensorData()  { return analogRead(LEFT_IR);  }
int IRSensor::rightSensorData() { return analogRead(RIGHT_IR); }
int IRSensor::frontSensorData() { return analogRead(FRONT_IR); }
