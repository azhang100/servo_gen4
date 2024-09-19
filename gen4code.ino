#include "Arduino.h"
#include "co2measure.h"
#include "pidfunction.h"
#include "MFCSerial.h"
<<<<<<< Updated upstream
#include "Wire.h"
#include "SparkFun_STC3x_Arduino_Library.h"  //Click here to get the library: http://librarymanager/All#SparkFun_STC3x
#include "SoftwareSerial.h"

// SoftwareSerial sws2(10, 11);
// SoftwareSerial sws1(12, 13);



MFCSerial mfc1(&Serial1);



//STC3x CO2Sensor;
//COZIR sensor1(&sws1);
// COZIR sensor2(&Serial2);
=======
#include "Wire.h"                            // DOWNLOAD THIS YOURSELF
#include "SparkFun_STC3x_Arduino_Library.h"  /// DOWNLOAD THIS YOURSELF
#include "SoftwareSerial.h"                  // DOWNLOAD THIS YOURSELF
#include "util.h"
#include "rtcsetup.h"
<<<<<<< Updated upstream
>>>>>>> Stashed changes
=======
//#include "Serial_multiplexer.h"

// SoftwareSerial sws2(10, 11);
//SoftwareSerial sws1(12, 13);



//MFCSerial mfc1(&Serial1);
SMP mfc1(&Serial2, 1, 9600);  // extends off serial port 1
SMP CO2Sensor(&Serial2, 4, 9600);
COZIR sensor1(&Serial2);

//COZIR sensor2(&Serial2);
>>>>>>> Stashed changes

#define START_DELIM '['
#define END_DELIM ']'
#define SPLIT_DELIM '='

SMP mfc1(&Serial2, 1, 9600); 
SMP CO2Sensor(&Serial2, 4, 9600);
COZIR sensor1(&Serial2);

<<<<<<< Updated upstream
char receivedChars[32];
char tempChars[32];
int tegco2 = 20;
=======
int tegco2 = 57;
int prev = 0;
>>>>>>> Stashed changes
long loops = 0;
long elapsed = 0;
float Kp = 0.1;
float Ki = 0.000002;
float Kd = 0;  //tuning
float sweep2 = 5;  //CHANGE VALUE FOR FIXED SWEEP
float fixed = 0.0;
bool newData = false;
bool looped = false;
bool fixedSweep = false;
<<<<<<< Updated upstream
String unitID = "A";
=======
bool resetI = false;
>>>>>>> Stashed changes


void setup() {

  PIDsetup();
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial2.begin(9600);
<<<<<<< Updated upstream
=======
  Serial2.print("Serial2 online");
<<<<<<< Updated upstream
  mfc1.switchPort(2); 
=======
  mfc1.switchPort(2);  //multiplexer port 8
>>>>>>> Stashed changes
  sensor1.CO2setup();
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  setrtc();
<<<<<<< Updated upstream
>>>>>>> Stashed changes

=======
  //digitalWrite(3, HIGH);
>>>>>>> Stashed changes
}

void loop() {
  while (true) {
<<<<<<< Updated upstream
=======
    //mfc1.setFlow(0.5);
    CO2Sensor.switchPort(4);
    double receivedCO2 = (sensor1.CO2loop() * 0.00076 * 12.38) - 1.9 - 5;  //calibrated to -5
    recvWithStartEndMarkers();
>>>>>>> Stashed changes

<<<<<<< Updated upstream
    recvWithStartEndMarkers();
    if (newData == true) {
      strcpy(tempChars, receivedChars);
      parseData();
      newData = false;
    }
=======
    printtime();
    recvUserInput();
    
>>>>>>> Stashed changes


<<<<<<< Updated upstream
    if (Serial1.available() > 0) {

      recvWithEndMarker(&Serial1);
      showNewNumber();

      if (looped) {
        continue;
      }

<<<<<<< Updated upstream
      sweep2 = PIDloop(currCO2c, tegco2, Kp, Ki, Kd);
=======
      sendCommand("altEgco2", (String)receivedCO2);
      //ln((String)receivedCO2);

      //prev = sensor1.CO2loop(prev);


      sweep2 = PIDloop(receivedCO2, tegco2, Kp, Ki, Kd, resetI);
      resetI = false;
>>>>>>> Stashed changes
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

<<<<<<< Updated upstream
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
      Serial.print("]");
      mfc1.setFlow(sweep2 / 10);

      int batteryVoltage = analogRead(A0);
      float hi = (batteryVoltage * 5 * 7.3) / 1024.0;
      Serial.print("[battery=");
      Serial.print(hi);
      Serial.println("]");

      looped = true;
=======
      float egco2 = getEGCO2(&Serial1);
      CO2Sensor.switchPort(4);
      double egco2_alt = (sensor1.CO2loop() * 0.00076 * 12.38) - 1.9;
  
      if (looped) { //prevents too many readings at once
        continue;
      }

      sweep2 = PIDloop(receivedCO2, tegco2, Kp, Ki, Kd, &resetI) * 10;
     
      if(loops % 1800 == 1799){ // every 30 mins
        blast();
      } 

      if(fixedSweep){
        sweep2 = fixed*10;
      } 
      
        mfc1.setFlow(sweep2);
        delay(200);

      
      looped = true;

      elapsed = millis() / 1000;
      sendCommand("egco2", (String)egco2);
      sendCommand("tegco2", (String)tegco2);
      sendCommand("ALTegco2", (String)egco2_alt);
      sendCommand("tSweep", (String)sweep2);
      sendCommand("loops", (String)loops);
      sendCommand("elapsed", (String)elapsed);
      Serial.println();
      Serial3.println();
      loops++;

>>>>>>> Stashed changes
=======
      sendCommand("tSweep", (String)sweep2);
      sendCommand("loops", (String)loops);
      // Serial.print("{loops=");
      // Serial.print(loops);
      // Serial.print("}");
      // Serial.println();
      // Serial1.print("{loops=");
      // Serial1.print(loops);
      // Serial1.println("}");
      // Serial1.println();
      loops++;
      elapsed = millis() / 1000;
      sendCommand("elapsed", (String)elapsed);
      // Serial.print("{elapsed=");
      // Serial.print(elapsed);
      // Serial.print("}");
      // Serial.println();
      // Serial1.print("{elapsed=");
      // Serial1.print(elapsed);
      // Serial1.print("}");
      // Serial1.println();

      //sendCommand("time", rtc.getTime());
      if (loops % 1800 == 1799) {
        blast();
      }  //every 30 mins
      mfc1.setFlow(sweep2 / 10);
      delay(200);
      looped = true;

      Serial.println();
      Serial1.println();

       String msg = Serial.readString();
       Serial.print(msg);
      // if (msg == "tegco2") {
      //   tegco2 = sendVal;
      // } else if (msg == "tSweep") {
      //   if (sendVal == -1) {
      //     fixedSweep = false;
      //     resetI = true;
      //   } else {
      //     fixedSweep = true;
      //     fixed = sendVal;
      //   }
      // }
>>>>>>> Stashed changes
    }
  }
}



<<<<<<< Updated upstream









float recvWithEndMarker(HardwareSerial *port) {
=======
void getEGCO2(HardwareSerial *port) {
>>>>>>> Stashed changes
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
  }
  if (newData == true) {
    float dataNumber = 0.000;
    dataNumber = atof(receivedChars);
    dataNumber = dataNumber * 7.6;  // THIS IS THE VALUE TO FILTER
    dataNumber = dataNumber + 9;     //calibrate with room air
    newData = false;
    looped = false;
  }

  return dataNumber;
  
}



void recvUserInput() {
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
<<<<<<< Updated upstream
<<<<<<< Updated upstream
=======

  if (newData == true) {
      strcpy(tempChars, receivedChars);
      parseData();
      newData = false;
    }

>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
}


void parseData() {  // split the data into its parts

  float changeVal;
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
<<<<<<< Updated upstream
  } else if (abc == "clear"){
      blast();
=======
  } else if (abc == "time") {
    //sendCommand("time", (String)(int)changeVal);
    //Serial.print("time=", )
    Serial.println("");
>>>>>>> Stashed changes
  }
<<<<<<< Updated upstream
  // strtokIndx = strtok(NULL, ",");
  // floatFromPC = atof(strtokIndx);  // convert this part to a float
=======
>>>>>>> Stashed changes
}


void blast(){
  float temp = mfc1.getFlow();
  mfc1.setFlow(2);
  delay(1000);
  mfc1.setFlow(temp);
}

<<<<<<< Updated upstream
void showNewNumber() {
  if (newData == true) {
    dataNumber = 0.000;
    dataNumber = atof(receivedChars);
<<<<<<< Updated upstream
    dataNumber = dataNumber * 7.6;  // THIS IS THE VALUE TO FILTER 
    currCO2c = dataNumber;
    Serial.print("[EGCO2=");
    Serial.print(dataNumber);
    Serial.print("]");
    Serial.print("[tEGCO2=");
    Serial.print(tegco2);
    Serial.print("]");
    //Serial.println(dataNumber);
=======
    dataNumber = dataNumber * 7.6;  // THIS IS THE VALUE TO FILTER
    currCO2c = dataNumber + 9;      //calibrate with room air
    //sendCommand("time", String(getTime()));
    // Serial.print("{time=");
    // //Serial.print(getTime());
    // Serial.print("}");
    // Serial.println();;
    // Serial1.print("{time=");
    // //Serial1.print(getTime());
    // Serial1.print("}");
    // Serial1.println();
    sendCommand("egco2", (String)currCO2c);
    sendCommand("tegco2", (String)tegco2);
    // Serial.print((String)currCO2c);
    // Serial.print(",");

>>>>>>> Stashed changes
    newData = false;
    looped = false;
  }
}
=======

>>>>>>> Stashed changes
