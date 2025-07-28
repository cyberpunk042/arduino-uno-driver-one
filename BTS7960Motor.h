#ifndef BTS7960MOTOR_H
#define BTS7960MOTOR_H

class BTS7960Motor {
private:
    int pwmPinRight;
    int pwmPinLeft;
    int targetSpeed;

    void writeSpeed(int speed) {
        speed = constrain(speed, -255, 255);  // Safety clamping

        if (speed > 0) {
            analogWrite(pwmPinRight, speed);
            analogWrite(pwmPinLeft, 0);
        } else if (speed < 0) {
            analogWrite(pwmPinRight, 0);
            analogWrite(pwmPinLeft, -speed);
        } else {
            analogWrite(pwmPinRight, 0);
            analogWrite(pwmPinLeft, 0);
        }
    }

public:
    BTS7960Motor(int pwmRight, int pwmLeft)
        : pwmPinRight(pwmRight), pwmPinLeft(pwmLeft), targetSpeed(0), currentSpeed(0) {}

    int currentSpeed;

    void setup() {
        pinMode(pwmPinRight, OUTPUT);
        pinMode(pwmPinLeft, OUTPUT);

        // Arduino Zero (SAMD21) – set PWM resolution if needed
        analogWriteResolution(8);  // Set to 8 bits: 0-255

        writeSpeed(0);
    }

    void setTargetSpeed(int speed) {
        targetSpeed = constrain(speed, -255, 255);
    }

    void updateSpeed() {
        // Simple ramping logic
        if (currentSpeed < targetSpeed) currentSpeed++;
        else if (currentSpeed > targetSpeed) currentSpeed--;
        writeSpeed(currentSpeed);
    }

    void stop() {
        currentSpeed = 0;
        targetSpeed = 0;
        writeSpeed(0);
    }
};

#endif
