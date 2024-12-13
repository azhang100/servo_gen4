#include "Arduino.h"  // DOWNLOAD THIS YOURSELF
#include "co2measure.h"
#include "pidfunction.h"
#include "MFCSerial.h"
#include "Wire.h"                            // DOWNLOAD THIS YOURSELF
#include "SparkFun_STC3x_Arduino_Library.h"  /// DOWNLOAD THIS YOURSELF
#include "SoftwareSerial.h"                  // DOWNLOAD THIS YOURSELF
#include "util.h"
//#include "rtcsetup.h"
//#include "Serial_multiplexer.h"

// SoftwareSerial sws2(10, 11);
//SoftwareSerial sws1(12, 13);



//MFCSerial mfc1(&Serial1);
SMP mfc1(&Serial2, 1, 9600);  // extends off serial port 1
SMP CO2Sensor(&Serial2, 4, 9600);
COZIR sensor1(&Serial2);

//COZIR sensor2(&Serial2);

#define START_DELIM '['
#define END_DELIM ']'
#define SPLIT_DELIM '='

const byte numChars = 32;
String toChange;

char receivedChars[numChars];
//char toChange[numChars];
char tempChars[numChars];  // TODO Andy: Can we make this a local variable in the parse() function?

int tegco2 = 48;
int prev = 0;
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
float changeVal;  // TODO Andy: Can we make this a local variable in the parse() function?
float fixed = 0.0;
bool newData = false;
bool fixedSweep = false;
bool resetI = false;
String unitID = "A";
//String currTime = "";


void setup() {

  PIDsetup();
  Serial.begin(9600);
  Serial.print("Serial online");
  Serial2.begin(9600);
  Serial2.print("Serial2 online");
  mfc1.switchPort(2);  //multiplexer port 8
  sensor1.CO2setup();
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);

  //  TODO Andy: Write out how Serial is connected here please
}

void loop() {
  Serial.print("startloop");

  CO2Sensor.switchPort(4);
  double receivedCO2 = (sensor1.CO2loop() * 0.00076 * 12.38) - 1.9 + 1.5 - 35 - 47;  // calibrated value
  recvSerial();
  recvSerial3();

  if (newData == true) {
    strcpy(tempChars, receivedChars);
    parseData();
    newData = false;
  }

  // if (Serial1.available() > 0) {  // TODO Andy: What is the purpose of this if-statement?

  // recvWithEndMarker(&Serial1);
  showNewNumber();

  // Serial1.println();

  Serial.print("halfwayThrough");
  sendCommand("egco2", (String)receivedCO2);

  sweep2 = PIDloop(currCO2c, tegco2, Kp, Ki, Kd, resetI);
  resetI = false;
  float temp = 0;


  if (fixedSweep) {
    sweep2 = fixed;
  }

  sendCommand("tSweep", (String)sweep2);
  sendCommand("loops", (String)loops);
  loops++;
  elapsed = millis() / 1000;
  sendCommand("elapsed", (String)elapsed);
  if (loops % 1800 == 1799) {
    blast();
  }  //every 30 mins
  mfc1.setFlow(sweep2 / 10);
  delay(900); // TODO Andy: Replace with millis() style delay

  //Serial.println();
  Serial3.println();
  Serial.println("endloop");
  // Serial1.println();
  // }
}


// TODO Andy: What is the purpose of recvWithEndMarker? Nothing is connected to Serial1 RX?
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

// TODO Andy: receivedChars is a global variable used by both recvSerial and recvSerial3
//            if you get a message on both Serial and Serial3 simultanously (one starts before the other ends)
//            receivedChars will be corrupted
//            That being said, this is not really that big a problem because chances are
//            only one channel will be used as input at a time
//            It's also very unlikely given that the messages are typically send in a fraction of a second
//            Code would be more "elegant" if the message were processed entirely in the function,
//            or the entire command once it got the endMarker. And then fed directly into the parse() function
//            That would reduce the number of global variables and flags.
void recvSerial() {
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

void recvSerial3() {
  static boolean recvInProgress = false;
  static byte ndx = 0;
  char startMarker = '[';
  char endMarker = ']';
  char rc;

  while (Serial3.available() > 0 && newData == false) {
    rc = Serial3.read();

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
    if (changeVal == -1) {
      fixedSweep = false;
      resetI = true;
    } else {
      fixedSweep = true;
      fixed = changeVal;
    }
  } else if (abc == "clear") {
    blast();
  } else if (abc == "sweepP") {
    Kp = changeVal;
  } else if (abc == "sweepI") {
    Ki = changeVal;
  } else if (abc == "sweepD") {
    Kd = changeVal;
  } else if (abc == "resetI") {
    if (changeVal == 1) {
      resetI = true;
    }
  } else if (abc == "time") {
    sendCommand("time", (String)(int)changeVal);
    Serial.println("");
  }

  // strtokIndx = strtok(NULL, ",");
  // floatFromPC = atof(strtokIndx);  // convert this part to a float
}


void blast() {
  float temp = mfc1.getFlow();
  mfc1.setFlow(2);
  delay(1000);
  mfc1.setFlow(temp);
}

void showNewNumber() {
  if (newData == true) {
    dataNumber = 0.000;
    dataNumber = atof(receivedChars);
    dataNumber = dataNumber * 7.6;   // THIS IS THE VALUE TO FILTER
    currCO2c = dataNumber + 9 + 71;  //calibrate with room air
    //printtime();
    sendCommand("altEgco2", (String)currCO2c);
    sendCommand("tegco2", (String)tegco2);

    newData = false;
  }
}
