#ifndef STANDARD_MOTOR_CONTROLLER_H
#define STANDARD_MOTOR_CONTROLLER_H

class StandardMotorController {
private:
    int in1Pin;           // First direction pin (IN1) - OUTPUT to motor controller
    int in2Pin;           // Second direction pin (IN2) - OUTPUT to motor controller
    int pwmPin;           // PWM pin for speed control - OUTPUT to motor controller
    int currentSpeed;
    int targetSpeed;
    bool isForward;
    bool isStopped;
    
    void writeSpeed(int speed) {
        speed = constrain(speed, 0, 255);  // Safety clamping
        
        if (speed == 0) {
            // Stop motor by setting both direction pins LOW
            digitalWrite(in1Pin, LOW);
            digitalWrite(in2Pin, LOW);
            isStopped = true;
        } else {
            isStopped = false;
            // INVERT the PWM value for this motor controller
            int invertedSpeed = 255 - speed;
            analogWrite(pwmPin, invertedSpeed);
        }
    }

public:
    StandardMotorController(int in1, int in2, int pwmOutputPin)
        : in1Pin(in1), in2Pin(in2), pwmPin(pwmOutputPin), 
          currentSpeed(0), targetSpeed(0), isForward(true), isStopped(true) {}

    void setup() {
        pinMode(in1Pin, OUTPUT);
        pinMode(in2Pin, OUTPUT);
        pinMode(pwmPin, OUTPUT);
        
        // Arduino Zero (SAMD21) – set PWM resolution if needed
        #if defined(ARDUINO_ARCH_SAMD)
            analogWriteResolution(8);  // Set to 8 bits: 0-255 (Only for SAMD/Zero)
        #endif
        
        // Initialize direction and speed
        setDirection(true);
        writeSpeed(0);
    }

    void update() {
        // Smooth speed ramping (only if not stopped)
        if (!isStopped) {
            if (currentSpeed < targetSpeed) {
                currentSpeed++;
            } else if (currentSpeed > targetSpeed) {
                currentSpeed--;
            }
            
            writeSpeed(currentSpeed);
        }
    }

    void setTargetSpeed(int speed) {
        targetSpeed = constrain(speed, 0, 255);
        
        if (speed == 0) {
            stop();
        } else {
            isStopped = false;
        }
    }

    void setDirection(bool forward) {
        // Always update direction, even when stopped
        if (forward != isForward) {
            // Stop motor briefly when changing direction to prevent damage
            if (!isStopped) {
                writeSpeed(0);
                delay(10);
            }
            isForward = forward;
        }
        
        // Set direction pins based on direction (regardless of stopped state)
        if (isForward) {
            digitalWrite(in1Pin, LOW);
            digitalWrite(in2Pin, HIGH);
        } else {
            digitalWrite(in1Pin, HIGH);
            digitalWrite(in2Pin, LOW);
        }
    }

    void setSpeedPin(int speed) {
        // Set PWM value on speed control pin (inverted)
        int safeSpeed = constrain(speed, 0, 255);
        if (safeSpeed == 0) {
            stop();
        } else {
            int invertedSpeed = 255 - safeSpeed;
            analogWrite(pwmPin, invertedSpeed);
        }
    }

    void stop() {
        currentSpeed = 0;
        targetSpeed = 0;
        isStopped = true;
        // Set both direction pins to LOW for brake/stop
        digitalWrite(in1Pin, HIGH);
        digitalWrite(in2Pin, HIGH);
        analogWrite(pwmPin, 255);
    }

    int getCurrentSpeed() const {
        return currentSpeed;
    }

    bool getDirection() const {
        return isForward;
    }
    
    bool isMotorStopped() const {
        return isStopped;
    }
};

#endif 