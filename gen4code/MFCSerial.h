/*
  Arduino Library for Serial MFC communication 

  To be used with SparkFun Transceiver Breakout - MAX3232
  https://www.sparkfun.com/products/11189

  -Rishi Tappeta
*/
#ifndef MFCSerial_h
#define MFCSerial_h
#endif


#include <inttypes.h>
#include <Wire.h>
#include "Print.h"
#include <HardwareSerial.h>
#include <SoftwareSerial.h>



class MFCSerial{

public:
  // Constructors
  MFCSerial(HardwareSerial* portA);
  MFCSerial(SoftwareSerial* portA);

  // MFC compatible functions
  void setFlow(float flow);
  void setupFlow(int baud, float max=5.0, String ID="A");
  float getFlow();

private:
  SoftwareSerial* portS;
  HardwareSerial* port;
  String unitID = "A";
  float maxFlow = 5.0;
  float currentFlow = 0;
  bool hardwareSerial;

  
};


