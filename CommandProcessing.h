// CommandProcessing.h
#ifndef COMMAND_PROCESSING_H
#define COMMAND_PROCESSING_H
#define Serial SerialUSB

struct MotorCommand {
    int leftSpeed;
    int rightSpeed;
    String status;
};

MotorCommand readMotorCommandFromSerial() {
    if (Serial.available()) {
        String input = Serial.readStringUntil('\n');
        MotorCommand cmd;
        Serial.println("Reading: " + input);

        int commaIndex = input.indexOf(',');
        if (commaIndex != -1) {
            String leftPart = input.substring(1, commaIndex);  // Extract left part
            String rightPart = input.substring(commaIndex + 2); // Extract right part

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
