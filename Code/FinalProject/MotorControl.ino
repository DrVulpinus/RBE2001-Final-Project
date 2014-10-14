
#include <Servo.h>

Servo rightDrive;
Servo leftDrive;
Servo armMain;
Servo armWrist;
Servo armClaw;

int armWristPos = 90;
void setupMotors(){
  rightDrive.attach(rightDrivePort);
  leftDrive.attach(leftDrivePort);
  armMain.attach(armMainPort);
  armWrist.attach(armWristPort);
  armClaw.attach(armClawPort);
  armMain.write(armMainPos);
  
}

void runMotors(int leftPower, int rightPower){

  rightDrive.write(map(rightPower,-100,100,0,180)); //map the input from a scale of -100 to 100 for ease of use, and then write it to the motor.
  leftDrive.write(map(leftPower,-100,100, 180,0)); //map the input from a scale of -100 to 100 for ease of use, and then write it to the motor.
}

void stopMotors(){
  rightDrive.write(90); //Stop the Right Motor
  leftDrive.write(90); //Stop the left Motor
}


//This function will raise the arm until it hits the limit switch -- around 24
void armMainRaise(){
  armMainPos = 24;
  while(digitalRead(swArmMainUpr)){
    //armMainPos--;
    armMain.write(armMainPos);
    Serial.print("Upper: ");
    Serial.print(digitalRead(swArmMainUpr));  
    Serial.print(" Lower: ");
    Serial.print(digitalRead(swArmMainLwr));
    Serial.print(" Position: ");
    Serial.print(armMainPos);
    Serial.println("");
    delay(100);
  }
}

//This function will lower the arm until it hits the limit switch -- around 112
void armMainLower(){
  armMainPos = 112;
  while(digitalRead(swArmMainLwr)){
    //armMainPos++;
    armMain.write(armMainPos);
    Serial.print("Upper: ");
    Serial.print(digitalRead(swArmMainUpr));  
    Serial.print(" Lower: ");
    Serial.print(digitalRead(swArmMainLwr));
    Serial.print(" Position: ");
    Serial.print(armMainPos);
    Serial.println("");
    delay(100);
  }
}


void armWristLower(){
  armWrist.write(70);
}
void armWristRaise(){
  armWrist.write(160);
}


void armClawOpen(){
  armClaw.write(25);
}
void armClawClose(){
  armClaw.write(180);
}

