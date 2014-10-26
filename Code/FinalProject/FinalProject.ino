

/*These #defines are for all of the various bluetooth message types and message data*/
#define msgTypeStorAvail   0x01
#define msgTypeSuppAvail   0x02
#define msgTypeRadAlert    0x03
#define msgTypeStopMove    0x04
#define msgTypeResMove     0x05
#define msgTypeRbtStatus   0x06
#define msgTypeHeartbeat   0x07


#define msgDataRadLow      0x2C
#define msgDataRadHigh     0xFF

#define rbtStatMoveStop    0x01
#define rbtStatMoveTeleop  0x02
#define rbtStatMoveAuto    0x03

#define rbtStatGripN       0x01
#define rbtStatGripY       0x02

#define rbtStatOpsGrip     0x01
#define rbtStatOpsUnGrip   0x02
#define rbtStatOpsDrvReac  0x03
#define rbtStatOpsDrvStor  0x04
#define rbtStatOpsDrvSupp  0x05
#define rbtStatOpsIdle     0x06

//Game States

#define gSFollowLineToReactor 0
#define gSRemoveTubeFromReactor 1
#define gSTurnAround 2
#define gSFollowLineToIntersection 3


//This section contains all of the configurations for everything on the robot
//This include the motor ports, sensor ports, and any other values which are set once at the beginning and not touched again
#define rightDrivePort 9
#define leftDrivePort 13
#define armMainPort 7
#define armWristPort 6
#define armClawPort 8
#define lightFL 0 //Front Left Light Sensor
#define lightFR 1 //Front Right Light Sensor
#define lightRL 2 //Rear Left Light Sensor
#define lightRR 3 //Rear Right Light Sensor
#define swArmMainUpr 23
#define swArmMainLwr 22
#define swFrontBumper 32
#define swEstop 25
#define swMessageGo 3



#define armMainDown 0
#define armMainUp 1
int armMainPos = armMainUp;

// set up module-wide variables
// these are 'volatile' as they are referenced in as well as outside of ISRs
volatile boolean sendHB = false;                       // flag indicating it is time to send a heartbeat message
volatile boolean sendRadLevel = false;                 // flag indicating it is time to send a radiation level message
volatile boolean go = false;                           // flag indicating it is OK to start transmitting messages
volatile boolean sendRbtStatus = false;
volatile unsigned char tickCount = 0;              // elapsed time in ticks (timer interrupts)
volatile unsigned long elapsedTime = 0;            // ...and in seconds
volatile unsigned char hbCount = 0;                // ticks since last heartbeat message was sent
volatile unsigned char radLevelCount = 0;          // ticks since last radiation level update
volatile unsigned char maxTimeCount = 0;




int autoState = 0;
// these need to be module-wide variables (so they persist across iterations of loop()
byte storageData;                              // holds the bitmask for the storage tubes
byte supplyData;                               // ... and the supply tubes

byte robotStatusDataOld[3];
int gameState =0;

//This variable stores the "X" position of the robot
//0 = Reactor A
//1 = Supply 1/Storage 4
//2 = Supply 2/Storage 3
//3 = Supply 3/Storage 2
//4 = Supply 4/Storage 1
//5 = Reactor B
int positionNumber = 0;

//This variable stores the "Y" position of the robot
#define fSStorage 0// = Storage Side
#define fSCenter 1// = Center Line
#define fSSupply 2// = Supply Side
int fieldSide = 1;

//This variable stores the heading of the robot
#define dirReactorA 0// = Towards Reactor A
#define dirSupplyTubes 1// = Towards Supply Tubes
#define dirReactorB 2// = Towards Reactor B
#define dirStorageTubes 3// = Towards Storage Tubes
int directionNumber = 0;


//These are the data variables, if these variables are changed, it will initiate sending of messages, lights, etc...
volatile unsigned char radLevelData = 0;           // the current radiation level on the robot
byte robotStatusData[3];

void setup() {
  Serial.begin(115200); //Start the Serial port
  setupMotors(); //Attach the motors to the correct ports
  //setupLCD();    //Setup and begin the LCD output
  setupTimer();  //Setup the timer code
  //setupPPM();

  pinMode(swArmMainUpr, INPUT_PULLUP);
  pinMode(swArmMainLwr, INPUT_PULLUP);
  pinMode(swFrontBumper, INPUT_PULLUP);
  pinMode(swEstop, INPUT_PULLUP);
  pinMode(swMessageGo, INPUT_PULLUP);
  delay(500);
}
boolean foundI = false;

boolean gotToStep2 = false;
void loop() {

  /*This bit of code handles all of the communications stuff that needs to happen in loop.
   Normally all message sends will happen from here, when the flag for that message is set by the timer.
   In the event that there is some blocking code in loop, if a message flag is set for more than 4.5 seconds,
   the timer interrupt will sent the message the next time the interrupt occurs to preserve the integety of the communications.
   The reason for this is to keep the processing time of the interrupt to a minimum, while still ensuring that messages get sent on time
   The only message type for which this does not apply is the robot status messages, these are only sent when the status changes,
   and if the staus changes, but there is some time before the status message gets sent, we shouldn't be as concerned as we would be with
   radiation alerts or heartbeats*/
  if (!digitalRead(swMessageGo)){
    go = true;
  }


  if (sendHB){      //If the flag for a heartbeat has been set
    sendMsgHB();    //Send it right now
    delay(20);
  }

  if (sendRadLevel){ //If the flag for sending a rad level has been set
    sendMsgRadUpdate(radLevelData);  //send it right now
    delay(20);
  }
  if (sendRbtStatus){
    sendMsgRobotStatus();
    delay(20);
  }



  //This switch runs the robot in each different mode, stopped, Teleop, and Autonomous


  if (digitalRead(swEstop)){
    stopRobot();
    Serial.print(" Estop Engaged ");
  }
  else{
    robotStatusData[0] = rbtStatMoveTeleop;

  }
  switch(robotStatusData[0]){
  case rbtStatMoveStop:
    //We are stopped, don't do nothin'!

    break;
  case rbtStatMoveTeleop:
    setArmPositions();
    Serial.print(" Teleop Mode ");
    printServoPositions();
    driveTeleop();
    break;
  case rbtStatMoveAuto:
    setArmPositions();
    Serial.print(" Auto Mode ");

    // Serial.println(storageData);
    autonomousStuff();




    break;
  default:
    //This should never happen, but if it does, we shouldn't do anything because it shouldn't happen, and therefore something must be borked
    break;
  }
  Serial.println("");  //Each loop increment the Serial ouput one line
}//End Loop

//This function is a simple helper function for getting a boolean status for a single storage tube
boolean isStoreTubeOpen(int tubeNum){
  return bitRead(storageData, tubeNum);
}

//This function is a simple helper function for getting a boolean status for a single supply tube
boolean isSupplyTubeOpen(int tubeNum){
  return bitRead(supplyData, tubeNum);
}

void autonomousStuff(){
  switch(gameState){
  case 0:
    if (goToReactorA()){
      gameState++;
    }   
    break;
  case 1:
    extractLow();
    gameState++;
    break;
  case 2:
    //if (gotToReactorB()){
    //gameState++;
    //}
    break;
  case 3:
    stopMotors();
    break;
  default:
    break; 
  }
}





















