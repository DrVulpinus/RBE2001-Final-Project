void setupMotors(){
  rightDrive.attach(rightDrivePort);
  leftDrive.attach(leftDrivePort);
}

void runMotors(int leftPower, int rightPower){
  
  rightDrive.write(map(rightPower,-100,100, 0,180)); //map the input from a scale of -100 to 100 for ease of use, and then write it to the motor.
  leftDrive.write(map(leftPower,-100,100, 0,180)); //map the input from a scale of -100 to 100 for ease of use, and then write it to the motor.
}
