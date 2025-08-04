#include "CommandProcessing.h"
#include "StandardMotorController.h"

// --- Motor Configuration ---
// Standard motor controller with two direction pins (IN1/IN2) and one PWM pin
StandardMotorController leftMotor(2, 3, 4);   // IN1, IN2, PWM pin
StandardMotorController rightMotor(5, 6, 7);  // IN1, IN2, PWM pin

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
    
    // Uncomment if using external control interface
    // leftControl.setup();
    // rightControl.setup();
    
//    receiver.setup();=
    leftMotor.setTargetSpeed(0);
    rightMotor.setTargetSpeed(0);
    Serial.begin(9600);
}

void updateMotorsSpeed() {
    leftMotor.update();
    rightMotor.update();
}

void handleSerialInput(unsigned long curTime) {
    if (curTime - lastSerialCheckTime >= SERIAL_CHECK_INTERVAL && Serial.available() > 0) {
        MotorCommand cmd = readMotorCommandFromSerial();
        
        printMotorCommand(cmd);
        if(cmd.status == "Left-Input"){
            handleMotorCommand(leftMotor, cmd.leftSpeed);
        }
        else if(cmd.status == "Right-Input"){
            handleMotorCommand(rightMotor, cmd.rightSpeed);
        }
        lastSerialCheckTime = curTime;
    }
}

void handleMotorCommand(StandardMotorController& motor, int speed) {
    if(speed == 0) {
        motor.stop();  // Stop the motor completely
    } else {
        // Determine direction based on speed sign
        bool forward = (speed > 0);
        motor.setDirection(forward);
        
        // Convert speed to absolute value for PWM
        int absSpeed = abs(speed);
        int pwmSpeed = convertDutyToPWM(absSpeed);
        motor.setTargetSpeed(pwmSpeed);
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

    handleSerialInput(curTime);
    
    if (curTime - lastMotorUpdateTime >= MOTOR_UPDATE_INTERVAL) {
        updateMotorsSpeed();
        lastMotorUpdateTime = curTime;
    }

    delay(1);  // Minimal delay to avoid busy looping
}

