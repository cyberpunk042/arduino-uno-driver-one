// LCDDisplay.h
#ifndef LCD_DISPLAY_H
#define LCD_DISPLAY_H

#include <LiquidCrystal_I2C.h>

extern int displayedLeftSpeed;
extern int displayedRightSpeed;
extern String statusMessage;

// Initialize the LCD at I2C address 0x27 for a 20 chars and 4 line display
LiquidCrystal_I2C lcd(0x27, 20, 4);

unsigned long lastLCDUpdate = 0;
const unsigned long lcdUpdateInterval = 200; // Update LCD every 200 milliseconds

void initLCD() {
    lcd.init();
    lcd.backlight();
}
void updateLCD() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("L: " + String(displayedLeftSpeed) + "S ");
    lcd.setCursor(0, 1);
    lcd.print("R: " + String(displayedRightSpeed) + "S ");
    lcd.setCursor(0, 2);
    lcd.print("");
    lcd.setCursor(0, 3);
    lcd.print("M:" + statusMessage);
}

void updateLCDSafely() {
    unsigned long currentMillis = millis();
    if (currentMillis - lastLCDUpdate >= lcdUpdateInterval) {

        updateLCD(); // Perform the actual LCD update

        lastLCDUpdate = currentMillis; // Reset the last update time
    }
}

  #endif
