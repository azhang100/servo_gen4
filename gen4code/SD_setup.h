#ifndef SDsetup_h
#define SDsetup_h

#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include "RTClib.h"

const int chipSelect = 10;
File logFile;
RTC_DS1307 rtc;
String data;

void rtcsetup() {
  // Start serial communication
  Serial.begin(9600);
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1)
      ;
  }

  if (!rtc.isrunning()) {
    Serial.println("RTC is NOT running, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));  // Set RTC to the date & time this sketch was compiled
  }
}

void rtc_write() {
  // Get current timestamp from RTC
  DateTime now = rtc.now();
  String timestamp = String(now.year(), DEC) + '/' + String(now.month(), DEC) + '/' + String(now.day(), DEC) + ' ' + String(now.hour(), DEC) + ':' + String(now.minute(), DEC) + ':' + String(now.second(), DEC);
  Serial.println("Timestamp: " + timestamp);
}
#endif