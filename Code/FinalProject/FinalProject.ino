

//This section contains all of the configurations for everything on the robot
  //This include the motor ports, sensor ports, and any other values which are set once at the beginning and not touched again
const int rightDrivePort = 12;
const int leftDrivePort = 13;
const int lightFL = 0; //Front Left Light Sensor
const int lightFR = 1; //Front Right Light Sensor
const int lightRL = 2; //Rear Left Light Sensor
const int lightRR = 3; //Rear Right Light Sensor


void setup() {
  Serial.begin(9600);
  setupMotors(); //Attach the motors to the correct ports
  setupLCD(); //Setup and begin the LCD output

}

void loop() {
  followLine(30, .05); //(Start Power = 10, pScale = 10)
  
}
