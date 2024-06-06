#include "CommandProcessing.h"
#include "BTS7960Motor.h"
#include "LCDDisplay.h"
#include "RFReceiver.h"

// Define your motor pins
BTS7960Motor leftMotor(4, 3); 
BTS7960Motor rightMotor(5, 6);
RFReceiver receiver(7, 8);

unsigned long lastSerialProcessing = 0;
const int serialCheckInterval = 30; 
unsigned long lastReceiverProcessing = 0;
const int receiverInterval = 40;
unsigned long lastMotorProcessing = 0; 
const int motorInterval = 3; 
String statusMessage = "Default";
String command = "";

void setup() {  
    leftMotor.setup();
    rightMotor.setup();
    receiver.setup();
    initLCD();
    updateLCD();
}

void setMotorsTarget(int leftSpeed, int rightSpeed) {
    Serial.print("Set target speed: " + String(leftSpeed) + ", " + String(rightSpeed));
    leftMotor.setTargetSpeed(leftSpeed);
    rightMotor.setTargetSpeed(rightSpeed);
}

void updateMotorsSpeed() {
  //Serial.print(" - Updating motors - \n");
  leftMotor.updateSpeed();
  rightMotor.updateSpeed();
}

void loop() {
    unsigned long curtime = millis();

    if (curtime - lastReceiverProcessing > receiverInterval) {
        command = receiver.receive();
        lastReceiverProcessing = curtime;
    }
    if (command != "") {
        //Serial.print("Command -> ");
        //Serial.println(command);
        statusMessage = "RfCMD->" + command;
        MotorCommand cmd = readMotorCommandFromRF(command);
        setMotorsTarget(cmd.leftSpeed, cmd.rightSpeed);
    }
    else{
        if (curtime - lastSerialProcessing > serialCheckInterval && Serial.available() > 0) {
            MotorCommand cmd = readMotorCommandFromSerial();
            setMotorsTarget(cmd.leftSpeed, cmd.rightSpeed);
            lastSerialProcessing = curtime;
        }
    }
    
    if (curtime - lastMotorProcessing > motorInterval) {
        updateMotorsSpeed();
        updateLCDSafely();
        lastMotorProcessing = curtime;
    }
    
    delay(1);
}
