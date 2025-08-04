#ifndef COMMAND_PROCESSING_H
#define COMMAND_PROCESSING_H

struct MotorCommand {
    int leftSpeed;
    int rightSpeed;
    String status;
};

MotorCommand readMotorCommandFromSerial() {
    String input = Serial.readStringUntil('\n');
    MotorCommand cmd;
    Serial.println("Reading from Serial: " + input);

    input.trim(); // remove whitespace and newline
    if (input.startsWith("L:") || input.startsWith("R:")) {
        int value = input.substring(2).toInt(); // after 'L:' or 'R:'
        if (input.charAt(0) == 'L') {
            cmd.leftSpeed = value;
            cmd.rightSpeed = 0;
            cmd.status = "Left-Input";
        } else {
            cmd.leftSpeed = 0;
            cmd.rightSpeed = value;
            cmd.status = "Right-Input";
        }
    } else {
        cmd.status = "Invalid format";
        cmd.leftSpeed = 0;
        cmd.rightSpeed = 0;
    }
    return cmd;
}

#endif
