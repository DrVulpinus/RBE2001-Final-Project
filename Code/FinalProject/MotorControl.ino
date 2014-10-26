
#include <Servo.h>

Servo rightDrive;
Servo leftDrive;
Servo armMain;
Servo armWrist;
Servo armClaw;

int armWristPos = 90;
int drvLeft = 0;
int drvRight = 0;

void setupMotors(){
  rightDrive.attach(rightDrivePort);
  leftDrive.attach(leftDrivePort);
  armMain.attach(armMainPort);
  armWrist.attach(armWristPort);
  armClaw.attach(armClawPort);
  armMain.write(armMainPos);

}

void runMotors(int leftPower, int rightPower){
  drvLeft = map(leftPower,100,-100, 180,0);//map the input from a scale of -100 to 100 for ease of use, and then write it to the motor.
  drvRight = map(rightPower,100,-100,0,180); //map the input from a scale of -100 to 100 for ease of use, and then write it to the motor.
  rightDrive.write(drvRight); 
  leftDrive.write(drvLeft);
}

void stopMotors(){
  rightDrive.write(90); //Stop the Right Motor
  leftDrive.write(90); //Stop the left Motor
  armMain.write(90); //Stop the arm Motor
}


//This function will raise the arm until it hits the limit switch -- around 24
void armMainRaise(){
  armMainPos = armMainUp;
}

//This function will lower the arm until it hits the limit switch -- around 112
void armMainLower(){
  armMainPos = armMainDown;
}


void setArmPositions(){
  switch (armMainPos){
  case armMainUp:
    if (digitalRead(swArmMainUpr)){
      armMain.write(55);
    }
    else{
      armMain.write(92);
    }
    break;
  case armMainDown:
    if (digitalRead(swArmMainLwr)){
      armMain.write(115);
    }
    else{
      armMain.write(90);
    }
    break;

  } 
}





void armWristLower(){
  armWrist.write(170);
}
void armWristRaise(){
  armWrist.write(85);
}


void armClawOpen(){
  armClaw.write(25);
}
void armClawClose(){
  armClaw.write(180);
}

void printServoPositions(){
  Serial.print(" Servo Status(Arm: ");
  Serial.print(armMain.read());
  Serial.print(" Wrist: ");
  Serial.print(armWristPos);
  Serial.print(" Left Drv: ");
  Serial.print(drvLeft);
  Serial.print(" Right Drv: ");
  Serial.print(drvRight);
  Serial.print(") ");
}



