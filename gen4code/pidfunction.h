#ifndef pidfunction_h
#define pidfunction_h
#include <HardwareSerial.h>

unsigned long currTime;
float sweep;
float prop;
float integral;
float deriv;
float error;


void PIDsetup(){
  currTime = millis();
  sweep = 0.1;
  prop = 0;
  integral = 0;
  deriv = 0;
  error = 0;
}

float PIDloop (float currCO2, int tegco2, float Kp, float Ki, float Kd, bool reset){
    deriv = Kd * (currCO2-tegco2-error)/(millis()-currTime);                // neg small number
    error = currCO2-tegco2; //  60
    prop = Kp * (currCO2-tegco2); // 60
    
    integral += Ki * (millis() - currTime) * (currCO2-tegco2); //66
    if (reset) {integral = 0;}
    //Serial.begin(9600);
    Serial.print("[sweepP="); Serial.print(prop); Serial.print("]");
    Serial.print("[sweepI="); Serial.print(integral); Serial.print("]");
    Serial.print("[sweepD="); Serial.print(deriv); Serial.print("]");
    Serial.print("[sweepE="); Serial.print(error); Serial.print("]");

 
    currTime=millis();

    sweep = deriv + integral + prop; //66
  
    if (sweep < 0.7){
      //integral = (0.5-(error-Kp));
      //integral += 1;
      sweep = 0.7;
    }
    if (sweep > 20){
      //integral = (5-(error*Kp));
      //integral -= 1000;
      sweep = 20;
    }
    
    return sweep;
}


#endif
