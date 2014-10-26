/*This file is for general robot control
 The functions contained within are for used as higher level controls for running the robot
 These funtions mainly deal with "robot status" level operations and do not directly interface with sensors
 and motors so much, but rather are used for general control of the robot
 */



byte prevMoveStatus = 0x01;

//This function is used if the robot's staus is to become "stopped"
void stopRobot(){
  prevMoveStatus = robotStatusData[0]; //Get the current robot movement state (this will be used when the stop flag is cleared)
  robotStatusData[0] = 0x01; //Set the robot status flag to stopped
  armWristRaise();
  delay(500);
  armClawOpen();
  stopMotors(); //Stop driving

}



//This function resumes robot movement as soon as it is called
void resumeRobot(){
  robotStatusData[0] = prevMoveStatus; //set the robot staus flag back to whatever it was doing before
}

boolean bumperHit(){
  if (digitalRead(swFrontBumper)){
    return false; 
  }
  else
  {
    return true; 
  }
}

boolean goToReactorA(){
    Serial.print(" Going to reactor A ");
  if(!goToCenterLine()){
    return false; 
  }
  else{
    turnToReactorA();
    return followToEnd(0); 
  }
}

boolean goToReactorB(){
  Serial.print(" Going to reactor B ");
  if(!goToCenterLine()){
    return false; 
  }
  else{
    turnToReactorB();
    return followToEnd(5); 
  }
}

boolean goToSupplyTube(int tube){
  goToPosition(tube);
  //turnToSupplyTubes();
}






boolean goToPosition(int destination){
  Serial.print("Our position is: (");
  Serial.print(positionNumber);
  Serial.print(") ");
  if (positionNumber != destination){//We aren't there yet
    if(!goToCenterLine()){ //We need to be on the centerLine to move along the X axis
      Serial.println("We can't do it, not on centerline");
      return false;
    }
    else{//We are on the centerline, now lets navigate to the final destination
      if (positionNumber > destination){//if we are above the final destination, we need to point ourselves towards Reactor A
        turnToReactorA();
        if (!checkForIntersection(200)){
          followLine(30, .7);
          return false;
        }
        else{
          while(checkForIntersection(200)){
            runMotors(30,30);
          }
          stopMotors();
          positionNumber --;
          return false;
        }        
      }
      else{//We must be below our final destination, so we need to point towards Reactor B
        turnToReactorB();

        if (!checkForIntersection(200)){
          followLine(30, .7);
          return false;
        }
        else{
          while(checkForIntersection(200)){
            runMotors(30,30);
          }
          stopMotors();
          positionNumber ++;
          return false;
        }     

      }//End the "we are below the final destination else"
    }//End the "we are on the centerline else"
  }//End the "are we there yet" if
  else{ //We are there
    Serial.println("----------------------------------------------------------WE MADE IT------------------------------------------------------------------------------------");
    Serial.println("----------------------------------------------------------WE MADE IT------------------------------------------------------------------------------------");
    Serial.println("----------------------------------------------------------WE MADE IT------------------------------------------------------------------------------------");
    Serial.println("----------------------------------------------------------WE MADE IT------------------------------------------------------------------------------------");
    Serial.println("----------------------------------------------------------WE MADE IT------------------------------------------------------------------------------------");
    Serial.println("----------------------------------------------------------WE MADE IT------------------------------------------------------------------------------------");
    Serial.println("----------------------------------------------------------WE MADE IT------------------------------------------------------------------------------------");
    Serial.println("----------------------------------------------------------WE MADE IT------------------------------------------------------------------------------------");
    Serial.println("----------------------------------------------------------WE MADE IT------------------------------------------------------------------------------------");
    Serial.println("----------------------------------------------------------WE MADE IT------------------------------------------------------------------------------------");
    Serial.println("----------------------------------------------------------WE MADE IT------------------------------------------------------------------------------------");
    Serial.println("----------------------------------------------------------WE MADE IT------------------------------------------------------------------------------------");
    Serial.println("----------------------------------------------------------WE MADE IT------------------------------------------------------------------------------------");
    Serial.println("----------------------------------------------------------WE MADE IT------------------------------------------------------------------------------------");
    Serial.println("----------------------------------------------------------WE MADE IT------------------------------------------------------------------------------------");
    Serial.println("----------------------------------------------------------WE MADE IT------------------------------------------------------------------------------------");
    Serial.println("----------------------------------------------------------WE MADE IT------------------------------------------------------------------------------------");
    Serial.println("----------------------------------------------------------WE MADE IT------------------------------------------------------------------------------------");
    Serial.println("----------------------------------------------------------WE MADE IT------------------------------------------------------------------------------------");
    Serial.println("----------------------------------------------------------WE MADE IT------------------------------------------------------------------------------------");
    Serial.println("----------------------------------------------------------WE MADE IT------------------------------------------------------------------------------------");
    Serial.println("----------------------------------------------------------WE MADE IT------------------------------------------------------------------------------------");
    Serial.println("----------------------------------------------------------WE MADE IT------------------------------------------------------------------------------------");
    Serial.println("----------------------------------------------------------WE MADE IT------------------------------------------------------------------------------------");
    Serial.println("----------------------------------------------------------WE MADE IT------------------------------------------------------------------------------------");
    Serial.println("----------------------------------------------------------WE MADE IT------------------------------------------------------------------------------------");
    Serial.println("----------------------------------------------------------WE MADE IT------------------------------------------------------------------------------------");
    Serial.println("----------------------------------------------------------WE MADE IT------------------------------------------------------------------------------------");
    Serial.println("----------------------------------------------------------WE MADE IT------------------------------------------------------------------------------------");
    Serial.println("----------------------------------------------------------WE MADE IT------------------------------------------------------------------------------------");

    return true;
  }
}



boolean goToCenterLine(){
  //First determine where we are relative to the center line, turn if necessary and if we are already on it, we should just return true
  switch(fieldSide){
  case fSCenter:
    return true; //We are already at the center, so just return true
    break;
  case fSSupply: //We are at the supply tubes, make sure we are pointing the right way
    turnToStorageTubes();
    break;
  case fSStorage: //We are at the storage tubes, make sure wer are pointing the right way
    turnToSupplyTubes();
  default:
    break; 
  }
  //We now know where we are, and we know we are pointing towards the center line, the only thing left to do is to drive to the next intersection
  if (!checkForIntersection(150)){
    followLine(30, .7);
    return false;
  }
  else{
    stopMotors();
    fieldSide = fSCenter;
    return true;
  }
}





boolean followToEnd(int endPos){
  if (digitalRead(swFrontBumper)){
    followLine(30, .7);
    return false;
  }
  else{
    stopMotors();
    positionNumber = endPos;
    return true;
  }

}



//The following functions are used to turn the robot to different orientations on the field

void turnToStorageTubes(){
  switch(directionNumber){
  case dirReactorA:
    turnRight90();

    break;
  case dirReactorB:
    turnLeft90();

    break;
  case dirStorageTubes:

    break;
  case dirSupplyTubes:
    turnRight90();
    turnRight90();
    break;
  default:
    break;  

  } 
  directionNumber = dirStorageTubes; //Set the new direction of the robot
}
void turnToSupplyTubes(){
  switch(directionNumber){
  case dirReactorA:
    turnLeft90();

    break;
  case dirReactorB:
    turnRight90();

    break;
  case dirStorageTubes:
    turnRight90();
    turnRight90();
    break;
  case dirSupplyTubes:

    break;
  default:
    break;  

  } 
  directionNumber = dirSupplyTubes; //Set the new direction of the robot
}

void turnToReactorA(){
  switch(directionNumber){
  case dirReactorA:


    break;
  case dirReactorB:
    turnRight90();
    turnRight90();
    break;
  case dirStorageTubes:
    turnLeft90();
    break;
  case dirSupplyTubes:
    turnRight90();
    break;
  default:
    break;  

  } 
  directionNumber = dirReactorA; //Set the new direction of the robot
}



void turnToReactorB(){

  switch(directionNumber){
  case dirReactorA:
    Serial.print("Turn to Reactor B: ");
    Serial.println(directionNumber);
    turnRight90();
    turnRight90();
    break;
  case dirReactorB:

    break;
  case dirStorageTubes:
    Serial.print("Turn to Reactor B: ");
    Serial.println(directionNumber);
    turnRight90();
    break;
  case dirSupplyTubes:
    Serial.print("Turn to Reactor B: ");
    Serial.println(directionNumber);
    turnLeft90();
    break;
  default:
    break;  

  } 
  directionNumber = dirReactorB; //Set the new direction of the robot
}

void extractLow(){
  armWristLower();
  delay(750);
  armClawOpen();
  delay(750);
  armMainLower();
  setArmPositions();
  delay(750);
  armClawClose();
  delay(750);
  armMainRaise();
  setArmPositions();
  delay(750);
}

void depositLow(){
  
}

void depositHigh(){
  
}

void extractHigh(){
  
}




