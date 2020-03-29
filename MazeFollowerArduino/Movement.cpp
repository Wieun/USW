#include "Movement.h"

Encoder  lEncoder(2,4);
Encoder  rEncoder(3,5);

void Movement::forward(int lSpeed, int rSpeed){
    analogWrite(ENABLE_LEFT, lSpeed);
    analogWrite(ENABLE_RIGHT, rSpeed);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    Serial.println("Forward");
}

void Movement::stop(int lSpeed, int rSpeed){
    analogWrite(ENABLE_LEFT, 0);
    analogWrite(ENABLE_RIGHT, 0);
}

void Movement::right(int lSpeed, int rSpeed){
    analogWrite(ENABLE_LEFT, lSpeed);
    analogWrite(ENABLE_RIGHT,rSpeed);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    Serial.println("Right");
}

void Movement::left(int lSpeed, int rSpeed){
    analogWrite(ENABLE_LEFT, lSpeed);
    analogWrite(ENABLE_RIGHT,rSpeed);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    Serial.println("Left");
}



void Movement::setup(){
    Serial.begin(9600);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    pinMode(ENABLE_LEFT, OUTPUT);
    pinMode(ENABLE_RIGHT, OUTPUT);
    stop(0,0);
}
