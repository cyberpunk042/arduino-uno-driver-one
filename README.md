# Arduino Uno Driver One

## Overview
This project controls two DC motors using standard high-quality motor controllers with the correct pin configuration. It is now fully compatible with the Arduino Uno R3 (ATmega328P, 5V logic).

## Hardware Setup

### Motor Controllers
- **Left Motor**: Standard motor controller with IN1/IN2 direction pins and PWM speed control
- **Right Motor**: Standard motor controller with IN1/IN2 direction pins and PWM speed control

### Pin Assignments
| Arduino Pin | Function | Motor Controller Connection |
|-------------|----------|----------------------------|
| Pin 2 | IN1 Direction Control | Left Motor IN1 pin |
| Pin 3 | IN2 Direction Control | Left Motor IN2 pin |
| Pin 4 | PWM Speed Control | Left Motor PWM pin |
| Pin 5 | IN1 Direction Control | Right Motor IN1 pin |
| Pin 6 | IN2 Direction Control | Right Motor IN2 pin |
| Pin 7 | PWM Speed Control | Right Motor PWM pin |

## Direction Control Logic

### Forward Direction
- **IN1**: HIGH
- **IN2**: LOW

### Backward Direction
- **IN1**: LOW
- **IN2**: HIGH

### Stop/Brake
- **IN1**: LOW
- **IN2**: LOW

### Speed Control
- **PWM Output**: 0-255 range (0 = stop, 255 = full speed)
- **Smooth Ramping**: Gradual speed changes to prevent jerky movement
- **Safety Clamping**: Values constrained to safe limits

## Serial Communication
The system supports serial commands for testing and remote control using the Arduino Uno's standard Serial interface (USB-to-serial via Serial Monitor).

### Serial Message Format
- **S** for Side (L & R)
- **XXX** For Speed (0 to 100 as percentage)
- **S:XXX**

### Examples
- `L:50` - Left motor forward 50%
- `R:75` - Right motor forward 75%
- `R:-75` - Right motor reverse 75%
- `L0` - Stop left motors

## Features

### Safety Features
1. **Direction Change Protection**: Motors stop briefly when changing direction
2. **Speed Ramping**: Smooth acceleration/deceleration
3. **Debouncing**: Prevents false triggers on direction GPIO
4. **Speed Clamping**: Ensures values stay within safe limits
5. **Brake Function**: Both direction pins set to LOW for immediate stop

### GPIO Control
- **Two Direction Pins**: IN1/IN2 for precise direction control
- **PWM Speed Control**: Smooth speed control from 0-255
- **Real-time Response**: Immediate reaction to GPIO changes

## Requirements
- Arduino Uno R3 (ATmega328P)
- LCD-I2C from Frank Hafele (optional)
- Standard motor controllers (L298N, TB6612FNG, or DRV8833 recommended)
- 12V DC motors

## Wiring
See `WIRING_DIAGRAM.md` for detailed wiring instructions and component recommendations.

## Usage
1. **GPIO Control**: Use IN1/IN2 pins for direction and PWM pin for speed
2. **Serial Control**: Send commands via Serial Monitor for testing
3. **Automatic Updates**: Motors update every 3ms for smooth operation
4. **For Uno**: Upload `arduino-uno-driver-one.ino` to your Arduino Uno R3

## Programming Interface

### Setting Direction
```cpp
leftMotor.setDirection(true);   // Forward (IN1=HIGH, IN2=LOW)
leftMotor.setDirection(false);  // Backward (IN1=LOW, IN2=HIGH)
```

### Setting Speed
```cpp
leftMotor.setTargetSpeed(128);  // 50% speed
leftMotor.setSpeedPin(255);     // Direct PWM control
```

### Stopping
```cpp
leftMotor.stop();  // Sets IN1=LOW, IN2=LOW, PWM=0
```

## Troubleshooting
- Check power supply connections
- Verify IN1/IN2 pin wiring to motor controllers
- Ensure PWM pins are properly connected
- Test each motor independently first
- Check that both direction pins aren't HIGH simultaneously