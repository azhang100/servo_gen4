
#include "Arduino.h"
#include "co2measure.h"
#include "pidfunction.h"
#include "MFCSerial.h"
#include "Wire.h"
#include "SparkFun_STC3x_Arduino_Library.h"  //Click here to get the library: http://librarymanager/All#SparkFun_STC3x
#include "SoftwareSerial.h"

SoftwareSerial sws2(10, 11);
SoftwareSerial sws1(12, 13);



MFCSerial mfc1(&Serial1);



//STC3x CO2Sensor;
COZIR sensor1(&sws1);
COZIR sensor2(&Serial2);

#define START_DELIM '['
#define END_DELIM ']'
#define SPLIT_DELIM '='

const byte numChars = 32;
String toChange;

char receivedChars[numChars];
//char toChange[numChars];
char tempChars[numChars]; 

int tegco2 = 20;
int time = 0;
float Kp = 0.1;
float Ki = 0.00001;
float Kd = 0.1;
float currCO2a;
float currCO2b;
float currCO2c;
float sweep2 = 0.5;  //CHANGE VALUE FOR FIXED SWEEP
float dataNumber = 0.0;
float changeVal;
bool newData = false;
bool looped = false;

void setup() {

  Serial.begin(9600);
  sws1.begin(9600);
  sws2.begin(9600);

  //Wire.begin();

  //tegco2 = tegco2/0.00076;
  sensor1.CO2setup();
  //sensor2.CO2setup();
  PIDsetup();
  mfc1.setupFlow(9600, 5.0, "A");

  Serial1.begin(9600);
  Serial2.begin(9600);
}

void loop() {

  while (true) {

    recvWithStartEndMarkers(); 
    if (newData == true) {
        strcpy(tempChars, receivedChars);
            // this temporary copy is necessary to protect the original data
            //   because strtok() used in parseData() replaces the commas with \0
        parseData();
        newData = false;
    }


    if (Serial1.available() > 0) {


      recvWithEndMarker(&Serial1);
      showNewNumber();

      if (looped) {
        continue;
      }
      // float prevCO2 = currCO2a;
      // currCO2a = sensor1.CO2loop();
      // currCO2a = currCO2a * 0.00076;

      // if (currCO2a < 0) {
      //   continue;
      // }
      // Serial.print((String)currCO2a);




      // Serial.print(",");



      // currCO2b = sensor2.CO2loop();
      // currCO2b = currCO2b * 0.00076;

      // //currCO2 = currCO2 * 1000 * 0.00000076;
      // // if(abs(prevCO2-currCO2 > 3)){
      // //   Serial.print("C");
      // //   continue;
      // // }
      // // //Serial.print("currCO2 -> ");
      // if (currCO2b < 0) {
      //   continue;
      // }
      // Serial.print((String)currCO2b);

      // Serial.print(",");

      sweep2 = PIDloop(currCO2c, tegco2, Kp, Ki, Kd);
      Serial.print("[sweep=");
      Serial.print((String)sweep2);
      Serial.print("]");
      Serial.print("[time=");
      Serial.print((String)time);
      Serial.println("]");
      time++;
      mfc1.setFlow(sweep2 / 10);

      looped = true;
    }

    // BELOW IS FIXED SWEEP (UNCOMMENT FOR FIXED SWEEP BUT COMMENT OUT ABOVE)

    //mfc1.setFlow(sweep/10);
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
  strcpy(tempChars, strtokIndx);         // copy it to messageFromPC

  strtokIndx = strtok(NULL, ",");  // this continues where the previous call left off
  changeVal = atof(strtokIndx);    // convert this part to an integer

  String abc(tempChars);
  if(abc == "tEGCO2"){
    tegco2 = changeVal;
  }
  // strtokIndx = strtok(NULL, ",");
  // floatFromPC = atof(strtokIndx);  // convert this part to a float
}




void showNewNumber() {
  if (newData == true) {
    dataNumber = 0.000;
    dataNumber = atof(receivedChars);
    dataNumber = dataNumber * 7.6;
    currCO2c = dataNumber;
    Serial.print("[EGCO2=");
    Serial.print(dataNumber);
    Serial.print("]");
    Serial.print("[tEGCO2=");
    Serial.print(tegco2);
    Serial.print("]");
    newData = false;
    looped = false;
  }
}
