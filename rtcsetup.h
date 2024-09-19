#include <Wire.h>    //needed because DS3231 uses I2C Bus
#include <RTClib.h>  //needed becuase we have ready-made functions of this libray

RTC_DS3231 rtc;  //the object rtc is created from the class RTC_DS3231
#define deviceAddress 0x68

void setrtc() {
  rtc.begin();
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));  //auto update from computer time
  //rtc.adjust(DateTime(2024, 8, 13, 9, 15, 17));//set date-time manualy:yr,mo,dy,hr,mn,sec
}

String getTime() {

  String time = "";
  DateTime nowTime = rtc.now();
  Wire.beginTransmission(deviceAddress);  //START, Roll Cal
  Wire.write(0x00);                       //set SEC Register address
  Wire.endTransmission();                 //Execute the above queued data, ACK, STOP
  Wire.requestFrom(deviceAddress, 3);     //SEC, MIN, and HRS to read from RTC as BCD
  byte bcdSeconds = Wire.read();
  byte bcdMinutes = Wire.read();
  byte bcdHours = Wire.read();
  //Show HRS
  time += String(bcdHours >> 4);
  time += String(bcdHours & 0x0F);
  time += ":";
  time += String(bcdMinutes >> 4);
  time += String(bcdMinutes & 0x0F);
  time += ":";
  time += String(bcdSeconds >> 4);
  time += String(bcdSeconds & 0x0F);
  return time;
  //Serial.print(time);

  // Serial.print(bcdHours >> 4);
  // Serial.print(bcdHours & 0x0F);
  // Serial.print(':');
  // //show MIN--
  // Serial.print(bcdMinutes >> 4);
  // Serial.print(bcdMinutes & 0x0F);
  // Serial.print(':');
  // //shiw SEC
  // Serial.print(bcdSeconds >> 4);
  // Serial.print(bcdSeconds & 0x0F);
  // Serial.println();
  delay(1000);
}