#include <HardwareSerial.h>
//#include <MFCSerial.h>

class SMP : public MFCSerial {
public:

    SMP(HardwareSerial *pt, uint8_t MPPort, uint8_t baud) : MFCSerial(pt){
        //backupPort->begin(baud);
        backupPort = pt;
        prt = MPPort;
        switchPort(MPPort);
    }



    void switchPort(uint8_t portA) {
        switch(portA){
            case 1:
                //Serial.print("to1");
                digitalWrite(2, LOW);
                digitalWrite(3, LOW);
                digitalWrite(5, LOW);
                break;
            case 2:
                //Serial.print("to2");
                digitalWrite(2, HIGH);
                digitalWrite(3, LOW);
                digitalWrite(5, LOW);
                break;
            case 3:
                //Serial.print("to3");
                digitalWrite(2, LOW);
                digitalWrite(3, HIGH);
                digitalWrite(5, LOW);
                break;
            case 4:
                //Serial.print("to4");
                digitalWrite(2, HIGH);
                digitalWrite(3, HIGH);
                digitalWrite(5, LOW);
                break;
            case 5:
                //Serial.print("to5");
                digitalWrite(2, LOW);
                digitalWrite(3, LOW);
                digitalWrite(5, HIGH);
                break;
            case 6:
                //Serial.print("to6");
                digitalWrite(2, HIGH);
                digitalWrite(3, LOW);
                digitalWrite(5, HIGH);
                break;
            case 7:
                //Serial.print("to7");
                digitalWrite(2, LOW);
                digitalWrite(3, HIGH);
                digitalWrite(5, HIGH);
                break;
            case 8:
                //Serial.print("to8");
                digitalWrite(2, HIGH);
                digitalWrite(3, HIGH);
                digitalWrite(5, HIGH); 
                break;
        }
    }

    void print(String message){
        switchPort(prt);
        backupPort->print(message);
    }

    void print(int message){
        switchPort(prt);
        backupPort->print(message);
    }

    void print(float message){
        switchPort(prt);
        backupPort->print(message);
    }

    void setFlow(float flow){
        switchPort(prt);
        MFCSerial::setFlow(flow);
    }



private: 

    uint8_t prt;

    HardwareSerial *backupPort;

   // MFCSerial mfc(*port);

};



// Create an instance of MySerial
// SMP MP1(1, 5); (Extending Serial 1)
// MP1.print("hi");
