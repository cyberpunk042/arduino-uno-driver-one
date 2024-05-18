// LCDDisplay.h
#ifndef LCD_DISPLAY_H
#define LCD_DISPLAY_H

#include <Wire.h> 
#include <LCD-I2C.h>

extern BTS7960Motor leftMotor; 
extern BTS7960Motor rightMotor;
extern String statusMessage;

// Initialize the LCD at I2C address 0x27 for a 20 chars and 4 line display
LCD_I2C lcd(0x27, 16, 4); 

unsigned long lastLCDUpdate = 0;
const unsigned long lcdUpdateInterval = 200; // Update LCD every 200 milliseconds

void initLCD() {
    //lcd.begin();
    //lcd.init();
    
    lcd.begin();
    lcd.display();
    lcd.backlight();
    lcd.blinkOff();
}
void updateLCD() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("L: " + String(leftMotor.currentSpeed) + "S ");
    lcd.setCursor(0, 1);
    lcd.print("R: " + String(rightMotor.currentSpeed) + "S ");
    lcd.setCursor(0, 2);
    lcd.print("");
    lcd.setCursor(0, 3);
    lcd.print("M:" + statusMessage);
}

void updateLCDSafely() {
    unsigned long currentMillis = millis();
    if (currentMillis - lastLCDUpdate >= lcdUpdateInterval) {
        //Serial.print("Updating Display");

        updateLCD(); // Perform the actual LCD update

        lastLCDUpdate = currentMillis; // Reset the last update time
    }
}

  #endif
