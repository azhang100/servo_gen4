
#include <Wire.h>
#include <SoftwareSerial.h>
#include "SparkFun_STC3x_Arduino_Library.h" //Click here to get the library: http://librarymanager/All#SparkFun_STC3x
#include "SparkFun_SHTC3.h"
STC3x mySensor;
SHTC3 mySensor2;
SoftwareSerial sws(3, 2);

void setup()
{
  Serial.begin(9600);
  Wire.begin();

  //mySensor.enableDebugging(); // Uncomment this line to get helpful debug messages on Serial

  if (mySensor.begin() == false)
  {
    Serial.println(F("Sensor not detected. Please check wiring. Freezing..."));
    while (1)
      ;
  }

  //We need to tell the sensor what binary gas and full range we are using
  //Possible values are:
  //  STC3X_BINARY_GAS_CO2_N2_100   : Set binary gas to CO2 in N2.  Range: 0 to 100 vol%
  //  STC3X_BINARY_GAS_CO2_AIR_100  : Set binary gas to CO2 in Air. Range: 0 to 100 vol%
  //  STC3X_BINARY_GAS_CO2_N2_25    : Set binary gas to CO2 in N2.  Range: 0 to 25 vol%
  //  STC3X_BINARY_GAS_CO2_AIR_25   : Set binary gas to CO2 in Air. Range: 0 to 25 vol%

  if (mySensor.setBinaryGas(STC3X_BINARY_GAS_CO2_AIR_100) == false)
  {
    Serial.println(F("Could not set the binary gas! Freezing..."));
    while (1)
      ;
  }

}

void loop()
{
  if (mySensor.measureGasConcentration()) // measureGasConcentration will return true when fresh data is available
  {

    float CO2 = mySensor.getCO2();
    Serial.println((String)CO2);
<<<<<<< Updated upstream
=======

    // int batteryVoltage = analogRead(A1);
    // float hi = (batteryVoltage * 5 * 7.3) / 1024.0;
    // Serial.print("[battery=");
    // Serial.print(hi);
    // Serial.println("]");

>>>>>>> Stashed changes
    // SHTC3_Status_TypeDef result = mySensor2.update();             // Call "update()" to command a measurement, wait for measurement to complete, and update the RH and T members of the object
    // printInfo();

  }

 
                                               // This function is used to print a nice little line of info to the serial port    
  

  delay(1000);
}


void printInfo()
{
  if(mySensor2.lastStatus == SHTC3_Status_Nominal)              // You can also assess the status of the last command by checking the ".lastStatus" member of the object
  {
    Serial.print("RH = "); 
    Serial.print(mySensor2.toPercent());                        // "toPercent" returns the percent humidity as a floating point number
    Serial.print("%, T = "); 
    Serial.print(mySensor2.toDegF());                           // "toDegF" and "toDegC" return the temperature as a flaoting point number in deg F and deg C respectively 
    Serial.println(" deg F"); 
  }
  
}