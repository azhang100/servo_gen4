float recvWithEndMarker(HardwareSerial *port) {
  char receivedChars[32];
  static byte ndx = 0;
  char endMarker = '\n';
  char rc;

  if (port->available() > 0) {
    rc = port->read();

    if (rc != endMarker) {
      receivedChars[ndx] = rc;
      ndx++;
      if (ndx >= numChars) {
        ndx = numChars - 1;
      }
    } else {
      receivedChars[ndx] = '\0';  // terminate the string
      ndx = 0;
      newData = true;
    }
    return atof(receivedChars);
  }
  return 0;
}



void recvWithStartEndMarkers(HardwareSerial *port) {
  char receivedChars[32];
  static boolean recvInProgress = false;
  static byte ndx = 0;
  char startMarker = '[';
  char endMarker = ']';
  char rc;

  while (port->available() > 0 && newData == false) {
    rc = port->read();

    if (recvInProgress == true) {
      if (rc != endMarker) {
        receivedChars[ndx] = rc;
        ndx++;
        if (ndx >= numChars) {
          ndx = numChars - 1;
        }
      } else {
        receivedChars[ndx] = '\0';  // terminate the string
        recvInProgress = false;
        ndx = 0;
        newData = true;
      }
    }

    else if (rc == startMarker) {
      recvInProgress = true;
    }
  }
}


void parseData() {  // split the data into its parts

  char *strtokIndx;  // this is used by strtok() as an index

  strtokIndx = strtok(tempChars, "=");  // get the first part - the string
  strcpy(tempChars, strtokIndx);        // copy it to messageFromPC

  strtokIndx = strtok(NULL, ",");  // this continues where the previous call left off
  changeVal = atof(strtokIndx);    // convert this part to an integer

  String abc(tempChars);

  if (abc == "tegco2") {
    tegco2 = changeVal;
  } else if (abc == "tSweep") {
    if (changeVal == -1) {
      fixedSweep = false;
      resetI = true;
    } else {
      fixedSweep = true;
      fixed = changeVal;
    }
  } else if (abc == "clear") {
    blast();
  } else if (abc == "sweepP") {
    Kp = changeVal;
  } else if (abc == "sweepI") {
    Ki = changeVal;
  } else if (abc == "sweepD") {
    Kd = changeVal;
  } else if (abc == "resetI") {
    if (changeVal == 1) {
      resetI = true;
    }
  } else if (abc == "time") {
    sendCommand("time", (String)(int)changeVal);
    Serial.println("");
  }



  // strtokIndx = strtok(NULL, ",");
  // floatFromPC = atof(strtokIndx);  // convert this part to a float
}




void showNewNumber() {
  if (newData == true) {
    dataNumber = 0.000;
    dataNumber = atof(receivedChars);
    dataNumber = dataNumber * 7.6;  // THIS IS THE VALUE TO FILTER
    currCO2c = dataNumber + 9;     //calibrate with room air
    sendCommand("egco2", (String)currCO2c);
    sendCommand("tegco2", (String)tegco2);
    newData = false;
    looped = false;
  }
}