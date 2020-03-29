#include "Robot.h"

Encoder  leftEncoder(2,4);
Encoder  rightEncoder(3,5);

void Robot::setup(){
    Serial.begin(9600);

    lWallPID.SetMode(AUTOMATIC);
    rWallPID.SetMode(AUTOMATIC);
    lEncoderPID.SetMode(AUTOMATIC);
    rEncoderPID.SetMode(AUTOMATIC);

    lWallPID.SetTunings(Kp, Ki, Kd);
    rWallPID.SetTunings(Kp, Ki, Kd);
    lEncoderPID.SetTunings(Kp, Ki, Kd);
    rEncoderPID.SetTunings(Kp, Ki, Kd);

    lWallPID.SetOutputLimits( -200, 200);
    rWallPID.SetOutputLimits( -200, 200);
    lEncoderPID.SetOutputLimits( -200, 200);
    rEncoderPID.SetOutputLimits( -200, 200);
    robotMovement.setup();
    sensor.setup();
}

void Robot::followPath(){

    lWallInput = sensor.leftSensorData();
    rWallInput = sensor.rightSensorData();
    fWallInput = sensor.frontSensorData();
    lEncoderInput = leftEncoder.read();
    rEncoderInput = rightEncoder.read();

      
    
    // if left sensor detects wall adjust to right
    if(lWallInput > wallSetpoint){
      lWallPID.Compute();
      lEncoderSetpoint = base - wallOutput;
      rEncoderSetpoint = base + wallOutput;
    }
    // if left sensor detects wall adjust to right
    if(lWallInput > wallSetpoint && fWallInput > wallSetpoint){
        lWallPID.Compute();
        robotMovement.right(lEncoderSetpoint, rEncoderSetpoint);
        //delay(300);
    }
    // if left sensor detects wall adjust to right
    if(rWallInput > wallSetpoint){
      rWallPID.Compute();
      lEncoderSetpoint = base + wallOutput;
      rEncoderSetpoint = base - wallOutput;
    }
    // if left sensor detects wall adjust to right
    if(rWallInput > wallSetpoint && fWallInput > wallSetpoint){
        rWallPID.Compute();
        robotMovement.left(lEncoderSetpoint, rEncoderSetpoint);
        //delay(300);
    }
    if(rWallInput < wallSetpoint && lWallInput < wallSetpoint && fWallInput < wallSetpoint){
      lWallPID.Compute();
      lEncoderSetpoint = base + wallOutput;
      rEncoderSetpoint = base + wallOutput;
    }
    if(rWallInput > wallSetpoint && lWallInput > wallSetpoint && fWallInput > wallSetpoint){
      lWallPID.Compute();
      robotMovement.stop(0,0);
    }

    lEncoderPID.Compute();
    rEncoderPID.Compute();
    robotMovement.forward(lEncoderSetpoint, rEncoderSetpoint);
}
