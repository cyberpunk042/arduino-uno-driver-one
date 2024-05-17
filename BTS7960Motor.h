// CommandProcessing.h
#ifndef BTS7960MOTOR_H
#define BTS7960MOTOR_H

class BTS7960Motor {
private:
    int pwmPinRight;
    int pwmPinLeft;
    int currentSpeed;
    int targetSpeed;

    void writeSpeed(int speed) {  // Internal method to write speed to pins
        if (speed >= 0) {
            analogWrite(pwmPinRight, speed);
            analogWrite(pwmPinLeft, 0);
        } else {
            analogWrite(pwmPinRight, 0);
            analogWrite(pwmPinLeft, -speed);
        }
    }

public:
    BTS7960Motor(int pwmRight, int pwmLeft) : pwmPinRight(pwmRight), pwmPinLeft(pwmLeft), currentSpeed(0), targetSpeed(0) {}

    void setup() {
        pinMode(pwmPinRight, OUTPUT);
        pinMode(pwmPinLeft, OUTPUT);
        writeSpeed(0);
    }

    void setTargetSpeed(int speed) {  // Set target speed
        targetSpeed = speed;
    }

    void updateSpeed() {  // Call this method periodically to update speed
        if (currentSpeed < targetSpeed) {
            currentSpeed++;  // Ramp up speed
        } else if (currentSpeed > targetSpeed) {
            currentSpeed--;  // Ramp down speed
        }
        writeSpeed(currentSpeed);
    }

    void stop() {
        targetSpeed = 0;
        currentSpeed = 0;
        writeSpeed(0);
    }
};

#endif