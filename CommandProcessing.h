#ifndef COMMAND_PROCESSING_H
#define COMMAND_PROCESSING_H

struct MotorCommand {
    int leftSpeed;
    int rightSpeed;
    String status;
};

/*int rfSpeedConversion(int rfSpeed) {
    return map(rfSpeed, 0, 5, 0, 255);
}

MotorCommand readMotorCommandFromRF(String command) {
    MotorCommand cmd;
    Serial.println("Reading from RF: " + command);

    int commaIndex = command.indexOf(',');
    if (commaIndex != -1) {
        String leftPart = command.substring(2, commaIndex);  // Extract left part after "L:"
        String rightPart = command.substring(commaIndex + 3); // Extract right part after "R:"

        if (command.charAt(0) == 'L' && command.charAt(commaIndex + 1) == 'R') {
            cmd.leftSpeed = rfSpeedConversion(leftPart.toInt());
            cmd.rightSpeed = rfSpeedConversion(rightPart.toInt());
            cmd.status = "OK";
        } else {
            cmd.status = "Invalid format";
        }
    } else {
        cmd.status = "Invalid command";
    }
    return cmd;
}*/

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
