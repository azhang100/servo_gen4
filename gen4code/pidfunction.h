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

float PIDloop (float currCO2, int tegco2, float Kp, float Ki, float Kd){
    deriv = Kd * (currCO2-tegco2-error)/(millis()-currTime);                // neg small number
    error = currCO2-tegco2; //  60
    prop = Kp * (currCO2-tegco2); // 60
    integral += Ki * (millis() - currTime) * (currCO2-tegco2); //66
    //Serial.begin(9600);
    Serial.print("[Deriv="); Serial.print(deriv); Serial.print("]");
    Serial.print("[Error="); Serial.print(error); Serial.print("]");
    Serial.print("[Prop="); Serial.print(prop); Serial.print("]");
    Serial.print("[Integral="); Serial.print(integral); Serial.print("]");

 
    currTime=millis();

    sweep = deriv + integral + prop; //66
  
    if (sweep < 0.5){
      //integral = (0.5-(error-Kp));
      //integral += 1;
      sweep = 0.5;
    }
    if (sweep > 20){
      //integral = (5-(error*Kp));
      //integral -= 1000;
      sweep = 20;
    }
    
    return sweep;
}


#endif
