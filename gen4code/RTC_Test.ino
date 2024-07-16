#include <Wire.h>
#include "RTClib.h"

RTC_DS1307 rtc;

void setup() {
  // Start serial communication
  Serial.begin(9600);
  while (!Serial) {
    // wait for serial port to connect. Needed for native USB
  }

  // Initialize RTC
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1) {
      delay(10); // Prevent watchdog resets in some cases
    }
  }

  // Check if the RTC is running, and set the time if it is not
  if (!rtc.isrunning()) {
    Serial.println("RTC is NOT running, setting the time!");
    // The following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  } else {
    Serial.println("RTC is running.");
  }
}

void loop() {
  // Get the current timestamp from the RTC
  DateTime now = rtc.now();

  // Format the timestamp as "YYYY/MM/DD HH:MM:SS"
  String timestamp = String(now.year(), DEC) + '/' + 
                     String(now.month(), DEC) + '/' + 
                     String(now.day(), DEC) + ' ' + 
                     String(now.hour(), DEC) + ':' + 
                     String(now.minute(), DEC) + ':' + 
                     String(now.second(), DEC);

  // Print the timestamp to the serial monitor
  Serial.println("Current Timestamp: " + timestamp);

  // Wait for a second before printing the next timestamp
  delay(1000);
}