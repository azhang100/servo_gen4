#ifndef Util_h
#define Util_h  // disabled (swap order of define and ifndef)



#include "Arduino.h"
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include "rtcsetup.h"
#include "Serial_multiplexer.h"

#define START_DELIM '['
#define END_DELIM ']'
#define SPLIT_DELIM '='

//=======================Serial choose==================

#define BTSERIAL Serial3 // long-range comms
#define DBSERIAL Serial

//=======================================================
void setupComms();
void acknowledgeCommand(String command, String arg);
void sendCommand(const String& command, const String& arg);
void loopComms(){};

//=======================================================

SMP portSwitcher(&Serial2, 2, 9600);

void setupComms() {
  DBSERIAL.print("setting up comms");
  DBSERIAL.println("set up BT");
  BTSERIAL.begin(9600);
  BTSERIAL.println("btSerial online");
  DBSERIAL.print("done");
}

void sendCommand(const String& command, const String& arg) {
  //portSwitcher.switchPort(2); uncomment if using multiplexer
  BTSERIAL.print(START_DELIM);
  BTSERIAL.print(command);
  BTSERIAL.print(SPLIT_DELIM);
  BTSERIAL.print(arg);
  BTSERIAL.print(END_DELIM);
  DBSERIAL.print(START_DELIM);
  DBSERIAL.print(command);
  DBSERIAL.print(SPLIT_DELIM);
  DBSERIAL.print(arg);
  DBSERIAL.print(END_DELIM);
}

void acknowledgeCommand(String command, String arg) {
  BTSERIAL.print("got cmd:");
  BTSERIAL.print(command);
  BTSERIAL.print("=");
  BTSERIAL.println(arg);
  DBSERIAL.print("got cmd:");
  DBSERIAL.print(command);
  DBSERIAL.print("=");
  DBSERIAL.println(arg);
}


#endif
