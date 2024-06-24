#include "Arduino.h" // DOWNLOAD THIS YOURSELF
#include "co2measure.h" 
#include "pidfunction.h"
#include "MFCSerial.h"
#include "Wire.h" // DOWNLOAD THIS YOURSELF
#include "SparkFun_STC3x_Arduino_Library.h"  /// DOWNLOAD THIS YOURSELF
#include "SoftwareSerial.h" // DOWNLOAD THIS YOURSELF

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
String unitID = "A";


void setup() {

  PIDsetup();
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial2.begin(9600);

}

void loop() {
  while (true) {

    recvWithStartEndMarkers();
    if (newData == true) {
      strcpy(tempChars, receivedChars);
      parseData();
      newData = false;
    }


    if (Serial1.available() > 0) {

      recvWithEndMarker(&Serial1);
      showNewNumber();

      if (looped) {
        continue;
      }

      sweep2 = PIDloop(currCO2c, tegco2, Kp, Ki, Kd);
      float temp = 0;
      if(loops % 900 == 899){
        temp = mfc1.getFlow();
        mfc1.setFlow(2);
        delay(500);
        mfc1.setFlow(temp);
        sweep2 = 20;
      }
      
      if (fixedSweep) {
        sweep2 = fixed;
      }
      Serial.print("[sweep=");
      Serial.print((String)sweep2);
      Serial.print("]");
      Serial.print("[loops=");
      Serial.print((String)loops);
      Serial.print("]");
      loops++;
      elapsed = millis()/1000;
      Serial.print("[elapsed=");
      Serial.print((String)elapsed);
      Serial.println("]");
      mfc1.setFlow(sweep2 / 10);
      looped = true;
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
}


void parseData() {  // split the data into its parts

  char *strtokIndx;  // this is used by strtok() as an index

  strtokIndx = strtok(tempChars, "=");  // get the first part - the string
  strcpy(tempChars, strtokIndx);        // copy it to messageFromPC

  strtokIndx = strtok(NULL, ",");  // this continues where the previous call left off
  changeVal = atof(strtokIndx);    // convert this part to an integer

  String abc(tempChars);

  if (abc == "tEGCO2") {
    tegco2 = changeVal;
  } else if (abc == "sweep") {
    fixed = changeVal;
  } else if (abc == "fixed") {
    if (changeVal == 1) {
      fixedSweep = true;
    } else if (changeVal == 0) {
      fixedSweep = false;
    }
  } else if (abc == "clear"){
      blast();
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
    currCO2c = dataNumber;
    Serial.print("[EGCO2=");
    Serial.print(dataNumber);
    Serial.print("]");
    Serial.print("[tEGCO2=");
    Serial.print(tegco2);
    Serial.print("]");
    //Serial.println(dataNumber);
    newData = false;
    looped = false;
  }
}
