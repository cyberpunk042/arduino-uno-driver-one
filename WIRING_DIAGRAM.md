# Standard Motor Controller Wiring Diagram (Two Direction Pins)

## Overview
This setup uses standard high-quality motor controllers with the correct pin configuration:
- **Two Direction Pins (IN1/IN2)**: One HIGH, one LOW for direction control
- **One PWM Pin**: For speed control (0-255)

## ⚠️ Important: Voltage Control Issue Resolved

The motor controller pins are **OUTPUT pins** that send signals TO the motor controller. They should NOT be used as INPUT pins to read external voltages. This was causing voltage conflicts.

### Solution: Separate Control Interface
- **Motor Controller Pins**: OUTPUT only (to motor controller)
- **External Control Pins**: INPUT only (from switches/potentiometers)
- **Control Logic**: Reads external inputs, then commands motor controller

## Components Needed

### For Each Motor:
1. **Standard Motor Controller** (e.g., L298N, TB6612FNG, or similar)
2. **Two Direction Pins**: IN1 and IN2 for forward/backward control (OUTPUT)
3. **One PWM Pin**: For speed control (OUTPUT)
4. **Motor** (12V DC motor recommended)

### Optional External Controls:
1. **Toggle Switch**: For direction control (INPUT)
2. **Potentiometer**: For speed control (INPUT)

## Wiring Diagram

### Motor Controller Setup (OUTPUT pins):
```
Arduino Zero Pin 2 (IN1) ──┐
                             ├── Motor Controller IN1 pin
Arduino Zero Pin 3 (IN2) ───┼── Motor Controller IN2 pin
                             │
Arduino Zero Pin 4 (PWM) ───┘
```

### Right Motor Controller Setup (OUTPUT pins):
```
Arduino Zero Pin 5 (IN1) ──┐
                             ├── Motor Controller IN1 pin
Arduino Zero Pin 6 (IN2) ───┼── Motor Controller IN2 pin
                             │
Arduino Zero Pin 7 (PWM) ───┘
```

### Optional External Control Setup (INPUT pins):
```
Arduino Zero Pin 8 ── Toggle Switch (Direction)
Arduino Zero Pin A0 ── Potentiometer (Speed)
```

## Pin Assignments

### Motor Controller Outputs (Arduino → Motor Controller)
| Arduino Pin | Function | Motor Controller Connection |
|-------------|----------|----------------------------|
| Pin 2 | IN1 Direction Control | Left Motor IN1 pin |
| Pin 3 | IN2 Direction Control | Left Motor IN2 pin |
| Pin 4 | PWM Speed Control | Left Motor PWM pin |
| Pin 5 | IN1 Direction Control | Right Motor IN1 pin |
| Pin 6 | IN2 Direction Control | Right Motor IN2 pin |
| Pin 7 | PWM Speed Control | Right Motor PWM pin |

### External Control Inputs (External → Arduino) - Optional
| Arduino Pin | Function | External Component |
|-------------|----------|-------------------|
| Pin 8 | Direction Input | Toggle Switch |
| Pin A0 | Speed Input | Potentiometer |

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

### Coast (Freewheel)
- **IN1**: LOW
- **IN2**: LOW
- **PWM**: 0

## Speed Control
- **PWM Output**: 0-255 range (0 = stop, 255 = full speed)
- **Smooth Ramping**: Gradual speed changes to prevent jerky movement
- **Safety Clamping**: Values constrained to safe limits

## Motor Controller Recommendations

### Option 1: L298N Dual H-Bridge
- **Pros**: Robust, handles high current, widely available
- **Cons**: Less efficient, requires heat sinking
- **Max Current**: 2A per channel
- **Control**: IN1/IN2 pins + PWM pin

### Option 2: TB6612FNG Dual Motor Driver
- **Pros**: More efficient, smaller footprint, built-in protection
- **Cons**: Lower current rating
- **Max Current**: 1.2A per channel
- **Control**: IN1/IN2 pins + PWM pin

### Option 3: DRV8833 Dual H-Bridge
- **Pros**: Very efficient, small size, good for battery operation
- **Cons**: Lower current rating
- **Max Current**: 1.7A per channel
- **Control**: IN1/IN2 pins + PWM pin

## Power Supply Requirements

- **Motor Voltage**: 12V DC (recommended)
- **Logic Voltage**: 5V (from Arduino)
- **Current Rating**: Depends on your motors (typically 1-3A per motor)

## Safety Features

1. **Direction Change Protection**: Motors stop briefly when changing direction
2. **Speed Ramping**: Smooth acceleration/deceleration
3. **Debouncing**: Prevents false triggers on direction GPIO
4. **Speed Clamping**: Ensures speed values stay within safe limits (0-255)
5. **Brake Function**: Both direction pins set to LOW for immediate stop
6. **Voltage Isolation**: Proper separation of input and output pins

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

### External Control (Optional)
```cpp
// Uncomment in main file to enable external control
MotorControlInterface leftControl(8, A0);
bool direction = leftControl.readDirection();
int speed = leftControl.readSpeed();
```

## Testing

1. **Direction Test**: Set IN1/IN2 pins correctly and verify motor direction changes
2. **Speed Test**: Vary PWM values and verify smooth speed control
3. **Stop Test**: Ensure motors stop when both direction pins are LOW
4. **Brake Test**: Verify immediate stop when stop() is called
5. **Voltage Test**: Verify no voltage conflicts between input and output pins

## Troubleshooting

- **Motor doesn't move**: Check power supply and motor connections
- **Wrong direction**: Verify IN1/IN2 pin connections are correct
- **Erratic behavior**: Check PWM pin wiring and motor controller setup
- **No speed control**: Verify PWM pin is properly connected
- **Motor runs continuously**: Check that both direction pins aren't HIGH simultaneously
- **Voltage conflicts**: Ensure input pins are separate from output pins 