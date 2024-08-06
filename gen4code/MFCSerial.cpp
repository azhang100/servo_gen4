/*
  Arduino Library for Serial MFC communication 

  To be used with SparkFun Transceiver Breakout - MAX3232
  https://www.sparkfun.com/products/11189

  -Rishi Tappeta
*/
#include "MFCSerial.h"
#include <inttypes.h>
#include <Wire.h>
#include "Print.h"

MFCSerial::MFCSerial(HardwareSerial *p){

  port = p;
  hardwareSerial = true;

}

MFCSerial::MFCSerial(SoftwareSerial *p){

  portS = p;

}

void MFCSerial::setFlowMP(float flow, HardwareSerial *p){

  if(flow > maxFlow){
      currentFlow = maxFlow;
      flow = 64000*(maxFlow/5);
      p->print("\r\r" + unitID + (String)flow + "\r");

      return;
    }
    currentFlow = flow;
    flow = (flow/5)*64000;
    p->print("\r\r" + unitID + (String)flow + "\r");

}

void MFCSerial::setFlow(float flow){

  if(hardwareSerial){
    //Serial.print("flow set");

    if(flow > maxFlow){
      currentFlow = maxFlow;
      flow = 64000*(maxFlow/5);
      port->print("\r\r" + unitID + (String)flow + "\r");

      return;
    }
    currentFlow = flow;
    flow = (flow/5)*64000;
    port->print("\r\r" + unitID + (String)flow + "\r");

  }
  else{
    if(flow > maxFlow){
      currentFlow = maxFlow;
      flow = 64000*(maxFlow/5);
      portS->print("\r\r" + unitID + (String)flow + "\r");

      return;
    }
    currentFlow = flow;
    flow = (flow/5)*64000;
    portS->print("\r\r" + unitID + (String)flow + "\r");

  }

}

void MFCSerial::setupFlow(int baud, float max=5.0, String ID="A"){
  if(hardwareSerial){
    port->begin(baud);
    maxFlow = max;
    unitID = ID;
    port->print("\r\r" + unitID + "@=" + ID + "\r"); 
  }
  else{
    portS->begin(baud);
    maxFlow = max;
    unitID = ID;
    portS->print("\r\r" + unitID + "@=" + ID + "\r");   
  }
}

float MFCSerial::getFlow(){
  return currentFlow;
}


