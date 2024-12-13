#include <Wire.h>     //needed because DS3231 uses I2C Bus
#include <RTClib.h>   //needed becuase we have ready-made functions of this libray

RTC_DS3231 rtc;     //the object rtc is created from the class RTC_DS3231
#define deviceAddress 0x68

void setrtc() {
    rtc.begin();
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));//auto update from computer time
    //rtc.adjust(DateTime(2018, 12, 23, 9, 15, 17));//set date-time manualy:yr,mo,dy,hr,mn,sec
}

void printtime(){
    Serial.println(); 
    DateTime nowTime = rtc.now();
    Wire.beginTransmission(deviceAddress); //START, Roll Cal
    Wire.write(0x00); //set SEC Register address
    Wire.endTransmission(); //Execute the above queued data, ACK, STOP
    Wire.requestFrom(deviceAddress, 3);   //SEC, MIN, and HRS to read from RTC as BCD
    byte bcdSeconds = Wire.read();
    byte bcdMinutes = Wire.read();
    byte bcdHours = Wire.read();
    //Show HRS
    DBSERIAL.print(bcdHours >> 4);
    DBSERIAL.print(bcdHours & 0x0F);
    DBSERIAL.print(':');
    //show MIN--
    DBSERIAL.print(bcdMinutes >> 4);
    DBSERIAL.print(bcdMinutes & 0x0F);
    DBSERIAL.print(':');
    //show SEC
    DBSERIAL.print(bcdSeconds >> 4);
    DBSERIAL.print(bcdSeconds & 0x0F);

    BTSERIAL.print(bcdHours >> 4);
    BTSERIAL.print(bcdHours & 0x0F);
    BTSERIAL.print(':');
    //show MIN--
    BTSERIAL.print(bcdMinutes >> 4);
    BTSERIAL.print(bcdMinutes & 0x0F);
    BTSERIAL.print(':');
    //show SEC
    BTSERIAL.print(bcdSeconds >> 4);
    BTSERIAL.print(bcdSeconds & 0x0F);
    //Serial.println();
    delay(1000);
}