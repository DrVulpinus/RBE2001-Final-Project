/*This file contains functions for use with the teleop mode, including the PPM code
These functions contain all of the necessary machinery to drive the robot under control from the transmitter remotely*/

#include <PPM.h>

int leftDriveTelePower = 0;
int rightDriveTelePower = 0;
PPM ppm(2);

void setupPPM(){
 ppm.setLow(2,-100);
 ppm.setHigh(2, 100);
 ppm.setLow(3,-100);
 ppm.setHigh(3,100);
}

void driveTeleop(){
  leftDriveTelePower = map(ppm.getChannel(3),0,180,100,-100);
  rightDriveTelePower = map(ppm.getChannel(2),0,180,-100,100);  
  runMotors(leftDriveTelePower, rightDriveTelePower);
}

void driveServos(){
  
}
