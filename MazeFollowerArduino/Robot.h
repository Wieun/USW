#ifndef ROBOT_H
#define ROBOT_H

#include <PID_v1.h>
#include "Movement.h"
#include "Encoder.h"
#include "IRSensor.h"
#include "Arduino.h"



class Robot{
    double lWallInput, rWallInput, fWallInput, 
    lEncoderInput, rEncoderInput, wallOutput, lEncoderOutput,
        rEncoderOutput, lEncoderSetpoint, rEncoderSetpoint;
    double wallSetpoint = 250;
    
    double Kp = 0.9, Ki = 0, Kd = 0.1;

    int base = -999;
    
    PID lWallPID;
    PID rWallPID;
    PID lEncoderPID;
    PID rEncoderPID;
    
    Movement robotMovement;
    IRSensor sensor;

public:
Robot():lWallPID(&lWallInput, &wallOutput, &wallSetpoint, Kp, Ki, Kd, DIRECT),
        rWallPID(&rWallInput, &wallOutput, &wallSetpoint, Kp, Ki, Kd, DIRECT),
        lEncoderPID(&lEncoderInput, &lEncoderOutput, &lEncoderSetpoint, Kp, Ki, Kd, DIRECT),
        rEncoderPID(&rEncoderInput, &rEncoderOutput, &rEncoderSetpoint, Kp, Ki, Kd, DIRECT) {}
        
    void setup();
    void followPath();
};
    
#endif // ROBOT_H
