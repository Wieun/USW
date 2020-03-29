#include "Robot.h"

Robot myRobot;
void setup() {
  // put your setup code here, to run once:
  myRobot.setup();
}

void loop() {
  // put your main code here, to run repeatedly:
  myRobot.followPath();
}
