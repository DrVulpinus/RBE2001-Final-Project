/*
 * This sketch is sample code that implements a 'robot' for the RBE 2001 final project.
 * The specific purpose of it is to provide a baseline level of Bluetooth connectivity for
 * a team in communicating with the field control computer.
 *
 * This code demonstrates the sending and receiving of sample reactor protocol packets.
 * Not all possible packets are shown in this code.
 *
 * After power up or reset, the robot should only receives messages. It should not be
 * sending any messages until the 'Go' pushbutton has been pressed.  This is to make it
 * easier for the master Bluetooth module to pair up with the slave module. Once the
 * pairing has been completed, then the user may press the Go button.  That should allow
 * the robot to begin sending messages to the field control computer as well as allowing
 * it to begin moving around the playing field.
 *
 * Originally written by Benzun on 15th September 2012
 * Extensively modified by C. Putnam, August, 2013
 *
 * Modified by:
 *   C. Putnam, 2/7/14: Added requirement for external 10K pullups on the RX1 & TX1 lines
 *                    : General cleanup of code
 *
 *  C. Putnam, 9/15/14: Fixed the robot addressing problem
 *
 * Bluetooth-related circuitry:
 * - BT RX pin to digital pin 18 (TX1)
 * - BT TX pin to digital pin 19 (RX1)
 *
 * Spent fuel rod circuitry:
 * - Storage tube 1 LED to digital pin 46
 * - Storage tube 2 LED to digital pin 48
 * - Storage tube 3 LED to digital pin 50
 * - Storage tube 4 LED to digital pin 52
 *
 * Supply fuel rod circuitry:
 * - Supply tube 1 LED to digital pin 47
 * - Supply tube 2 LED to digital pin 49
 * - Supply tube 3 LED to digital pin 51
 * - Supply tube 4 LED to digital pin 53
 * 
 * Other I/O pins
 * - 'Go' LED on digital pin 2
 * - 'Go' pushbutton on digital pin 3
 */

// necessary include files
#include <BluetoothClient.h>
#include <BluetoothMaster.h>
#include <ReactorProtocol.h>


//various useful definitions
#define thisROBOT 19                           // <== CHANGE TO MATCH YOUR TEAM NUMBER!!!

#define goLED       2                          // pin for LED to indicate the robot is now OK'd to send data
#define goSW        3                          // pin for momentary pushbutton to initiate sending of data

#define onboardLED 13
#define spareLED   21                          // extra LED for <<whatever>>

#define storLED1  46                           // lights if storage tube bit 1 is set
#define storLED2  48                           // lights if storage tube bit 2 is set
#define storLED4  50                           // lights if storage tube bit 4 is set
#define storLED8  52                           // lights if storage tube bit 8 is set

#define splyLED1  47                           // lights if supply tube bit 1 is set
#define splyLED2  49                           // lights if supply tube bit 2 is set
#define splyLED4  51                           // lights if supply tube bit 4 is set
#define splyLED8  53                           // lights if supply tube bit 8 is set



// instantiate needed objects
ReactorProtocol pcol(byte(thisROBOT));         // instantiate the protocol object and set the robot/team source address
BluetoothClient bt;                            // instantiate a Bluetooth client object
BluetoothMaster btmaster;                      // ...and a master object




/*
 * setup() runs once on power-up or reset
 */
void setupBT(){
  // the slave bluetooth module is also configured to 115200 baud
  Serial1.begin(115200);
}


//Sends the appropriate radiation warning based on the value of radLevel as well set the robot status
//The robot status is set assuming that if the radiation level is more than 0, then we must have a rod in the gripper
// 0 = no radiation wanring
// 1 = low radiation warning
// 2 = high radiation warning
void sendMsgRadUpdate(int radLevel){   
  switch(radLevel){
  case 0:
    robotStatusData[1] = rbtStatGripN;
    break;
  case 1:
    robotStatusData[2] = rbtStatGripY;
    sendMsgRadLow();
    break;
  case 2:
    robotStatusData[2] = rbtStatGripY;
    sendMsgRadHigh();
    break;
  default:
    break;    
  }
}

//This function sends a heartbeat message when it is called
void sendMsgHB(){
  //Serial.println("Sending HB");
  byte pkt[10];                                // allocate memory for the bytes in the packet
  int sz;                                      // holds the size of the message (in bytes)
  byte type;                                   // hold the message type id
  byte data1[3];                               // holds any data associated with a message
  sendHB = false;                              // clear the heartbeat flag

  // generate and send the heartbeat message    
  digitalWrite(onboardLED, !digitalRead(onboardLED));  // flip the state of the HB LED
  pcol.setDst(0x00);			                // this will be a broadcast message
  sz = pcol.createPkt(msgTypeHeartbeat, data1, pkt);              // create a packet using the heartbeat type ID (there is no data)
  btmaster.sendPkt(pkt, sz);                          // send to the field computer
}

//This function sends a high radiation alert message
void sendMsgRadHigh(){
  byte pkt[10];                                // allocate memory for the bytes in the packet
  int sz;                                      // holds the size of the message (in bytes)
  byte type;                                   // hold the message type id
  byte data1[3];                               // holds any data associated with a message
  sendRadLevel = false;                              // clear the heartbeat flag

  // generate and send the heartbeat message    
  digitalWrite(onboardLED, !digitalRead(onboardLED));  // flip the state of the HB LED
  pcol.setDst(0x00);			               // this will be a broadcast message
  data1[0] = msgDataRadHigh;                                     // indicate a new fuel rod
  sz = pcol.createPkt(msgTypeRadAlert, data1, pkt);               // create a packet using the heartbeat type ID (there is no data)
  btmaster.sendPkt(pkt, sz);                           // send to the field computer
}

//This function sends a low radiation alert message
void sendMsgRadLow(){
  byte pkt[10];                                // allocate memory for the bytes in the packet
  int sz;                                      // holds the size of the message (in bytes)
  byte type;                                   // hold the message type id
  byte data1[3];                               // holds any data associated with a message
  sendRadLevel = false;                              // clear the heartbeat flag
  pcol.setDst(0x00);			               // this will be a broadcast message
  data1[0] = msgDataRadLow;                                     // indicate a new fuel rod
  sz = pcol.createPkt(msgTypeRadAlert, data1, pkt);               // create a packet using the heartbeat type ID (there is no data)
  btmaster.sendPkt(pkt, sz);                           // send to the field computer
}

//This function sends a low radiation alert message
void sendMsgRobotStatus(){
  byte pkt[10];                                // allocate memory for the bytes in the packet
  int sz;                                      // holds the size of the message (in bytes)
  byte type;                                   // hold the message type id
  byte data1[3];                               // holds any data associated with a message
  sendRbtStatus = false;
  pcol.setDst(0x00);			               // this will be a broadcast message
  data1[0] = robotStatusData[0];
  data1[1] = robotStatusData[1];
  data1[2] = robotStatusData[2];
  sz = pcol.createPkt(msgTypeRbtStatus, data1, pkt);   // create a packet using the heartbeat type ID (there is no data)
  btmaster.sendPkt(pkt, sz);                           // send to the field computer
}


void getMessages(){  
  byte pkt[10];                                // allocate memory for the bytes in the packet
  int sz;                                      // holds the size of the message (in bytes)
  byte type;                                   // hold the message type id
  byte data1[3];                               // holds any data associated with a message
  if (btmaster.readPacket(pkt)) {              // if we have received a message
  Serial.print(" Got Message ");
    if (pcol.getData(pkt, data1, type)) {      // see if we can extract the type and data
      switch (type) {                          // process the message based on the type
      case msgTypeStorAvail:                               // received a storage tube message
        storageData = data1[0];                // extract and save the storage-related data (the byte bitmask)
        break;
      case msgTypeSuppAvail:                               // received a supply tube message
        supplyData = data1[0];                 // extract and save the supply-related data (the byte bitmask)
        break;
      case msgTypeStopMove:
        stopRobot();                           //If we recieve a stopRobot message, drop everything and stop it now
        break;
      case msgTypeResMove:
        resumeRobot();
        break;
      default:                                 // ignore other types of messages
        break;
      }
    }
  }
}





