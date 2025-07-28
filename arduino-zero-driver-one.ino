#include "CommandProcessing.h"
#include "BTS7960Motor.h"
#include "RFReceiver.h"

// --- Motor Configuration ---
BTS7960Motor leftMotor(4, 3);   // IN1, IN2
BTS7960Motor rightMotor(5, 6);  // IN1, IN2

// --- RF Receiver ---
//RFReceiver receiver(7, 8);  // RX, TX

// --- Timings ---
constexpr unsigned long SERIAL_CHECK_INTERVAL   = 30;
constexpr unsigned long RECEIVER_CHECK_INTERVAL = 40;
constexpr unsigned long RECEIVER_TIMEOUT        = 500;
constexpr unsigned long MOTOR_UPDATE_INTERVAL   = 3;

// --- State Tracking ---
unsigned long lastSerialCheckTime   = 0;
unsigned long lastReceiverCheckTime = 0;
unsigned long lastMotorUpdateTime   = 0;

String statusMessage = "Default";
String command = "";

void setup() {
    leftMotor.setup();
    rightMotor.setup();
//    receiver.setup();=
    //leftMotor.setTargetSpeed(200);
    //rightMotor.setTargetSpeed(200);
    Serial.begin(9600);
}

void updateMotorsSpeed() {
    leftMotor.updateSpeed();
    rightMotor.updateSpeed();
}

/*void handleRFReceiver(unsigned long curTime) {
    if (curTime - lastReceiverCheckTime >= RECEIVER_CHECK_INTERVAL) {
        command = receiver.receive();

        if (!command.isEmpty()) {
            statusMessage = "RF CMD -> " + command;
            MotorCommand cmd = readMotorCommandFromRF(command);
            printMotorCommand(cmd);
            setMotorsTarget(cmd.leftSpeed, cmd.rightSpeed);
            lastReceiverCheckTime = curTime;
        } 
        else if (curTime - lastReceiverCheckTime >= RECEIVER_TIMEOUT) {
            // Failsafe: Stop motors on signal loss
            setMotorsTarget(0, 0);
        }
    }
}*/

void handleSerialInput(unsigned long curTime) {
    if (curTime - lastSerialCheckTime >= SERIAL_CHECK_INTERVAL && Serial.available() > 0) {
        MotorCommand cmd = readMotorCommandFromSerial();
        
        printMotorCommand(cmd);
        if(cmd.status == "Left-Input"){
            leftMotor.setTargetSpeed(convertDutyToPWM(cmd.leftSpeed));
        }
        else {
            rightMotor.setTargetSpeed(convertDutyToPWM(cmd.rightSpeed));
        }
        lastSerialCheckTime = curTime;
    }
}

int convertDutyToPWM(int dutyCycle) {
    return constrain(map(dutyCycle, 0, 100, 0, 255), 0, 255);
}


void printMotorCommand(const MotorCommand& cmd) {
    Serial.print("MotorCommand ->");
    Serial.print(" | Left: ");
    Serial.print(cmd.leftSpeed);
    Serial.print(" | Right: ");
    Serial.println(cmd.rightSpeed);
    Serial.print(" | Status: ");
    Serial.println(cmd.status);
}

void loop() {
    unsigned long curTime = millis();
    /*if (receiver.available()) {
        String msg = receiver.receive();
        MotorCommand cmd = readMotorCommandFromRF(msg);
        if (cmd.status == "OK") {
            setMotorsTarget(cmd.leftSpeed, cmd.rightSpeed);
            statusMessage = "From RF";
        }
    }*/

    MotorCommand serialCmd = readMotorCommandFromSerial();
    if (serialCmd.status == "Left-Input" || serialCmd.status == "Right-Input" ) {
        handleSerialInput(serialCmd);
        statusMessage = "From Serial";
    }

    if (curTime - lastMotorUpdateTime >= MOTOR_UPDATE_INTERVAL) {
        updateMotorsSpeed();
        lastMotorUpdateTime = curTime;
    }

    delay(1);  // Minimal delay to avoid busy looping
}
