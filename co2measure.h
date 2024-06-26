/*
AN128_ardunio_cozir CO2 Demonstration code.
 Runs on Ardunio UNO, MEGA or MEGA2560
 Written by: Marv Kausch 11/22/2016 for Co2meter.com
 Revised 11/29/17, 2/14/18 by John Houck
 This sketch connects a COZIR sensor and reports readings back to the host
computer over USB.
 The value is stored in a global variable 'co2' and can be used for any
number of applications.
 pin connections:
 Arduino________COZIR Sensor
 GND ------------------ 1 (gnd)
 3.3v------------------- 3 (Vcc)
 13 -------------------- 5 (Rx)
 12 -------------------- 7 (Tx)
*/
#ifndef co2measure_h
#define co2measure_h

#include <SoftwareSerial.h>

class COZIR{


private:

  double co2 =0;
  //String val= ""; //holds the string of the value
  double multiplier = 1;// 1 for 2% =20000 PPM, 10 for 20% = 200,000 PPM
  // double multiplier2  = 10;
  uint8_t buffer[25];
  uint8_t ind =0;
  uint8_t index = 0;
  SoftwareSerial *mySerial;
  HardwareSerial *mySerial2;
  bool hardware = false;
  // double fill_buffer(); // function prototypes here
  // double format_output();


public:

  COZIR(SoftwareSerial *port1){
    mySerial = port1;
  }

  COZIR(HardwareSerial *port1){
    mySerial2 = port1;
    hardware = true;
  }

  void CO2setup() {
    if(!hardware){
      Serial.begin(9600);
      Serial.print("\n\n");
      //Serial.println(" Ardunio to COZIR CO2 Sensor - Demonstration code 2/14/18\n\n");
      mySerial->begin(9600); // Start serial communications with sensor
      //mySerial.println("K 0"); // Set Command mode
      mySerial->println("M 6"); // send Mode for Z and z outputs
      // "Z xxxxx z xxxxx" (CO2 filtered and unfiltered)
      mySerial->println("K 1"); // set streaming mode
    }
    else{
      Serial.begin(9600);
      Serial.print("\n\n");
      //Serial.println(" Ardunio to COZIR CO2 Sensor - Demonstration code 2/14/18\n\n");
      mySerial2->begin(9600); // Start serial communications with sensor
      //mySerial.println("K 0"); // Set Command mode
      mySerial2->println("M 6"); // send Mode for Z and z outputs
      // "Z xxxxx z xxxxx" (CO2 filtered and unfiltered)
      mySerial2->println("K 1"); // set streaming mode
    }
  }


  double CO2loop() {

    //Serial.print("HI");
    fill_buffer(); // function call that reacds CO2 sensor and fills buffer
    //Serial.print("Buffer contains: ");
    //for(int j=0; j<ind; j++)Serial.print(buffer[j],HEX);
    index = 0;
    format_output();
    //Serial.print(" Raw PPM ");
    index = 8; // In ASCII buffer, filtered value is offset from raw by 8 bytes
    return format_output();

    //Serial.println(" Filtered PPM\n\n");
  }

  double fill_buffer(void){
  // Fill buffer with sensor ascii data
    if(!hardware){
      ind = 0;
      while(buffer[ind-1] != 0x0A){
        //Serial.print("sdfs");
        //Serial.println("dsfsd"); // Read sensor and fill buffer up to 0XA = CR
        if(mySerial->available()){
          
        buffer[ind] = mySerial->read();
            //Serial.print("Here -> " + buffer[ind]);

        ind++;
        }
      }
    }
    else{
      ind = 0;
      while(buffer[ind-1] != 0x0A){
        //Serial.print("sdfs");
        //Serial.println("dsfsd"); // Read sensor and fill buffer up to 0XA = CR
        if(mySerial2->available()){
          
        buffer[ind] = mySerial2->read();
            //Serial.print("Here -> " + buffer[ind]);

        ind++;
        }
      }
    }
    // buffer() now filled with sensor ascii data
    // ind contains the number of characters loaded into buffer up to 0xA = CR
      ind = ind -2; // decrement buffer to exactly match last numerical character
  }

  double format_output(void){ // read buffer, extract 6 ASCII chars, convert to PPM and print
    co2 = buffer[15-index]-0x30;
    co2 = co2+((buffer[14-index]-0x30)*10);
    co2 +=(buffer[13-index]-0x30)*100;
    co2 +=(buffer[12-index]-0x30)*1000;
    co2 +=(buffer[11-index]-0x30)*10000;
    // Serial.print("[egco2=");
    //Serial.println(co2*multiplier,0);
    // Serial.println("]");
  // Serial.print(" PPM,");
  // Serial.print("\n");
    //delay(200);
    //Serial.println(co2);
    
    return co2;
  }

};




 


 
  

 #endif
