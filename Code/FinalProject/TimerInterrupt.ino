#include <TimerOne.h>
/*
This file is for al timer interrupt based stuff
 The timer is setup so that it will tick once every 100 msec
 By setting up different counters it is possible to have different events triggered at different intervals
 
 Because this is primarily used for bluetooth communications, ideally, the actuall communication will occur
 not inside of the interrupt, but will rather occur in loop().  However, the interrupt has an additional feature
 which ensures that if some long running process in loop() delays sending a message for too long, that the
 messages will be sent from the interrupt to ensure integrety of the commmunications
 */




void setupTimer(){
  // set up the timer (it starts automatically)
  Timer1.initialize(100000);	               // set up a 100 millisecond timer period
  Timer1.attachInterrupt(timer1ISR);           // ...and specify the timer ISR 
}




void timer1ISR(void) {
  // we are here because the timer expired and generated an interrupt

  /*The following code handles counters and timing of various events as well as setting of message send flags
   */
  tickCount++;                                 // increment the 100ms tick counter
  hbCount++;                                   // increment the heartbeat counter
  radLevelCount++;
  if (tickCount == 10) {                       // do the following once a second
    tickCount = 0;                             // reset the tick counter
    elapsedTime++;			       // increment the elapsed time counter (in seconds)
  }
  if (hbCount == 20) {                         // do the following every other second
    hbCount = 0;                               // reset the heartbeat counter
    sendHB = true;                             // note it is time to send a heartbeat message
  }
  if (radLevelCount == 10){                    //do the following every second
    radLevelCount = 0;                         //reset the radiation level counter
    if (radLevelData > 0){
      sendRadLevel = true;                       //note that it is time to send a radiation level message
    }
  }


  /*This block of code handles robot status messages
   Essentially this code very simply watches the "live" robot status variable and compares it to a
   "stored" robot status variable.  If there is a difference between the "live" status and the "stored"
   status, it sets a flag which indicates that the robot status data should be sent to the FCS.
   Because robot status messages are optional, and do not have any time critical element to them,
   we only send them when there is a change in the status, and when there is time available in loop.
   The only time when we do not follow this protocol is with a stop message, if a stop message is received
   we need to do something about it right there, because a stopped message is blocking for all operations*/

  if(robotStatusData != robotStatusDataOld){ //If the staus has changed
    robotStatusDataOld[0] = robotStatusData[0];  //Update the old status data to reflect the recent changes
    robotStatusDataOld[1] = robotStatusData[1];
    robotStatusDataOld[2] = robotStatusData[2];
    sendRbtStatus = true; //set a flag to send a status update at the next available time
  }

  //The following code handles what to do if a a message flag has been set for some time,
  //but the messge has not been sent.  This section monitors the flags, and if a flag is set
  //it starts counting, if the the counter gets to the maximum allowable time without sending a message,
  //the message will be sent immediately from the interrupt loop to preserve communications integrity
  if (sendRadLevel || sendHB){
    maxTimeCount++;                              // increment the maximum Time between sends counter
  }
  else{
    maxTimeCount = 0; 
  }

  if (maxTimeCount == 45){                     //if 4.5 seconds are allowed to pass without
    //message being sent, we will need to force it to happen now
    if (sendHB){      //If there is a queued heartbeat message
      sendMsgHB();    //Send it right now
    }

    if (sendRadLevel){ //If there is a queued radiation alert message
      sendMsgRadUpdate(radLevelData);  //send it right now
    }

  }
  getMessages(); //This function checks for new messages and then performs initial actions based on the message type and data


} // end timer1ISR





