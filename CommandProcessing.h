#ifndef COMMAND_PROCESSING_H
#define COMMAND_PROCESSING_H
#define Serial SerialUSB

struct MotorCommand {
    int leftSpeed;
    int rightSpeed;
    String status;
};

int rfSpeedConversion(int rfSpeed) {
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
}

MotorCommand readMotorCommandFromSerial() {
    if (Serial.available()) {
        String input = Serial.readStringUntil('\n');
        MotorCommand cmd;
        Serial.println("Reading from Serial: " + input);

        int commaIndex = input.indexOf(',');
        if (commaIndex != -1) {
            String leftPart = input.substring(2, commaIndex);  // Extract left part after "L:"
            String rightPart = input.substring(commaIndex + 3); // Extract right part after "R:"

            if (input.charAt(0) == 'L' && input.charAt(commaIndex + 1) == 'R') {
                cmd.leftSpeed = leftPart.toInt();
                cmd.rightSpeed = rightPart.toInt();
                cmd.status = "OK";
            } else {
                cmd.status = "Invalid format";
            }
        } else {
            cmd.status = "Invalid command";
        }
        return cmd;
    } else {
        return MotorCommand{0, 0, "No data"};
    }
}

#endif
