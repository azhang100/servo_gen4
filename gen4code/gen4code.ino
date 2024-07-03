#include "Arduino.h" // DOWNLOAD THIS YOURSELF
#include "co2measure.h" 
#include "pidfunction.h"
#include "MFCSerial.h"
#include "Wire.h" // DOWNLOAD THIS YOURSELF
#include "SparkFun_STC3x_Arduino_Library.h"  /// DOWNLOAD THIS YOURSELF
#include "SoftwareSerial.h" // DOWNLOAD THIS YOURSELF
#include "util.h"
// SoftwareSerial sws2(10, 11);
// SoftwareSerial sws1(12, 13);



MFCSerial mfc1(&Serial1);



//STC3x CO2Sensor;
//COZIR sensor1(&sws1);
// COZIR sensor2(&Serial2);

#define START_DELIM '['
#define END_DELIM ']'
#define SPLIT_DELIM '='

const byte numChars = 32;
String toChange;

char receivedChars[numChars];
//char toChange[numChars];
char tempChars[numChars];

int tegco2 = 20;
long loops = 0;
long elapsed = 0;
float Kp = 0.1;
float Ki = 0.000002;
float Kd = 0;  //tuning
float currCO2a;
float currCO2b;
float currCO2c;
float sweep2 = 5;  //CHANGE VALUE FOR FIXED SWEEP
float dataNumber = 0.0;
float changeVal;
float fixed = 0.0;
bool newData = false;
bool looped = false;
bool fixedSweep = false;
bool resetI = false;
String unitID = "A";
String currTime = "";


void setup() {

  PIDsetup();
  Serial.begin(9600);
  Serial.print("Serial online");
  Serial1.begin(9600);
  Serial2.begin(9600);
  Serial2.print("Serial2 online");

}

void loop() {
  while (true) {

    recvWithStartEndMarkers();
    if (newData == true) {
      strcpy(tempChars, receivedChars);
      parseData();
      newData = false;
    } else {
      //Serial.println("no data");
    }

    if (Serial1.available() > 0) {

      recvWithEndMarker(&Serial1);
      showNewNumber();

      if (looped) {
        continue;
      }

      sweep2 = PIDloop(currCO2c, tegco2, Kp, Ki, Kd, resetI);
      resetI = false;
      float temp = 0;
      
      
      if (fixedSweep) {
        sweep2 = fixed;
      }

      // if(loops % 400 == 399){
      //   temp = mfc1.getFlow();
      //   mfc1.setFlow(2);
      //   delay(800);
      //   mfc1.setFlow(temp);
      //   sweep2 = 20;
      // }
      sendCommand("tSweep", (String)sweep2);
      sendCommand("loops", (String)loops);
      loops++;
      elapsed = millis()/1000;
      sendCommand("elapsed", (String)elapsed);
      mfc1.setFlow(sweep2 / 10);
      looped = true;
      float battery = analogRead(A0);
      battery = (battery * 5*7.3) / 1024.0;
      sendCommand("battery",(String)battery);
      Serial.println(); 
      Serial2.println();
    }
  }
}



float recvWithEndMarker(HardwareSerial *port) {
  static byte ndx = 0;
  char endMarker = '\n';
  char rc;

  if (port->available() > 0) {
    rc = port->read();

    if (rc != endMarker) {
      receivedChars[ndx] = rc;
      ndx++;
      if (ndx >= numChars) {
        ndx = numChars - 1;
      }
    } else {
      receivedChars[ndx] = '\0';  // terminate the string
      ndx = 0;
      newData = true;
    }
    return atof(receivedChars);
  }
  return 0;
}



void recvWithStartEndMarkers() {
  static boolean recvInProgress = false;
  static byte ndx = 0;
  char startMarker = '[';
  char endMarker = ']';
  char rc;

  while (Serial.available() > 0 && newData == false) {
    rc = Serial.read();

    if (recvInProgress == true) {
      if (rc != endMarker) {
        receivedChars[ndx] = rc;
        ndx++;
        if (ndx >= numChars) {
          ndx = numChars - 1;
        }
      } else {
        receivedChars[ndx] = '\0';  // terminate the string
        recvInProgress = false;
        ndx = 0;
        newData = true;
      }
    }

    else if (rc == startMarker) {
      recvInProgress = true;
    }
  }
  while (Serial2.available() > 0 && newData == false) {
    rc = Serial2.read();

    if (recvInProgress == true) {
      if (rc != endMarker) {
        receivedChars[ndx] = rc;
        ndx++;
        if (ndx >= numChars) {
          ndx = numChars - 1;
        }
      } else {
        receivedChars[ndx] = '\0';  // terminate the string
        recvInProgress = false;
        ndx = 0;
        newData = true;
      }
    }

    else if (rc == startMarker) {
      recvInProgress = true;
    }
  }
}


void parseData() {  // split the data into its parts

  char *strtokIndx;  // this is used by strtok() as an index

  strtokIndx = strtok(tempChars, "=");  // get the first part - the string
  strcpy(tempChars, strtokIndx);        // copy it to messageFromPC

  strtokIndx = strtok(NULL, ",");  // this continues where the previous call left off
  changeVal = atof(strtokIndx);    // convert this part to an integer

  String abc(tempChars);

  if (abc == "tegco2") {
    tegco2 = changeVal;
  } else if (abc == "tSweep") {
    if(changeVal == -1){
      fixedSweep = false;
      resetI =  true;
    }
    else{
      fixedSweep = true;
      fixed = changeVal;
    }
  } else if (abc == "clear"){
      blast();
  } else if (abc == "sweepP"){
      Kp = changeVal;
  } else if (abc == "sweepI"){
      Ki = changeVal;
  } else if (abc == "sweepD"){
      Kd = changeVal;
  } else if (abc == "resetI"){
      if(changeVal == 1){
        resetI = true;
      }
  }


  // strtokIndx = strtok(NULL, ",");
  // floatFromPC = atof(strtokIndx);  // convert this part to a float
}


void blast(){
  float temp = mfc1.getFlow();
  mfc1.setFlow(2);
  delay(1000);
  mfc1.setFlow(temp);
}

void showNewNumber() {
  if (newData == true) {
    dataNumber = 0.000;
    dataNumber = atof(receivedChars);
    dataNumber = dataNumber * 7.6;  // THIS IS THE VALUE TO FILTER 
    currCO2c = dataNumber+50; //calibrate with room air
    sendCommand("egco2", (String)currCO2c);
    sendCommand("tegco2", (String)tegco2);
    
    newData = false;
    looped = false;
  }
}
