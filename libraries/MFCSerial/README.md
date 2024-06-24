# Mass Flow Controller Commmunication Arduino library

Arduino Library for Serial MFC communication 

To be used with SparkFun Transceiver Breakout - MAX3232
https://www.sparkfun.com/products/11189

-Rishi Tappeta


## Functions

The following public functions are exposed.

### setFlow(float flow)

### setupFlow(int baud, float max=5.0, String ID="A")

### getFlow()
 


  
## Example usage

To use, include MFCSerial.h in your sketch and instantiate a new MFCSerial instance.


```c++
#include <MFCSerial.h>

// Create new MFCSerial instance
MFCSerial mfc1(&Serial1);

void setup() {

  // Set up the MFC Flow with Baud Rate, Max Flow, and Unit ID
  mfc1.setupFlow(38400, 5.0, "A");


  // Begin the serial that's associated with the controller (trying to integrate this but its being weird)
  Serial1.begin(38400); 


  // Initialize the port powering the breakout board (if not off standard 5V supply) - in this case it is 4
  // Don't need these lines if the breakout is powered off of standard 5V supply
  pinMode(4, OUTPUT);
  digitalWrite(4, HIGH);

}

void loop() {

    // Set the flow rate to 0.2 SLM
  mfc1.setFlow(0.2);


  // Random delay cuz why not
  delay(1000);


  // Set the flow rate to 0.6 SLM
  mfc1.setFlow(0.6);


  // Random delay cuz why not
  delay(1000);

}
```







