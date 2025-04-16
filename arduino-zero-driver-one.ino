#include "CommandProcessing.h"
#include "BTS7960Motor.h"
#include "LCDDisplay.h"
#include "RFReceiver.h"
#include <mcp_can.h>
#include <SPI.h>

const int CAN_CS_PIN = 9;
MCP_CAN CAN(CAN_CS_PIN); // Set CS pin

bool canAvailable = false;

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
    Serial.begin(115200);
    while (CAN_OK != CAN.begin(CAN_500KBPS)) {
        Serial.println("CAN BUS Init Failed");
        delay(100);
    }
    Serial.println("CAN BUS Init Success");
    
    leftMotor.setup();
    rightMotor.setup();
    receiver.setup();
    initLCD();
    updateLCD();
    
    canAvailable = true;
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
        statusMessage = "RfCMD->" + command;
        MotorCommand cmd = readMotorCommandFromRF(command);
        setMotorsTarget(cmd.leftSpeed, cmd.rightSpeed);
    } 
    else if (curtime - lastSerialProcessing > serialCheckInterval && Serial.available() > 0) {
        MotorCommand cmd = readMotorCommandFromSerial();
        setMotorsTarget(cmd.leftSpeed, cmd.rightSpeed);
        lastSerialProcessing = curtime;
    }
    
    // 🔽 CAN COMMAND HANDLING
    if (canAvailable && CAN_MSGAVAIL == CAN.checkReceive()) {
        unsigned char len = 0;
        unsigned char buf[8];
        CAN.readMsgBuf(&len, buf);
        int leftSpeed = (int16_t)((buf[1] << 8) | buf[0]);  // Little endian
        int rightSpeed = (int16_t)((buf[3] << 8) | buf[2]);

        setMotorsTarget(leftSpeed, rightSpeed);
        statusMessage = "CAN->" + String(leftSpeed) + "/" + String(rightSpeed);
    }

    if (curtime - lastMotorProcessing > motorInterval) {
        updateMotorsSpeed();
        updateLCDSafely();
        lastMotorProcessing = curtime;
    }

    delay(1);
}

