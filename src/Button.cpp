#include <Arduino.h>
#include <Preferences.h> 
#include "WiFiconfig.h"
#include "button.h"

static unsigned long btnPressTime = 0;  // Timestamp variable for the config button
static bool wifiCleared = false;        // Status flags for the config button
static Preferences prefs;               

void buttonInit() {
  // Pull-up resistor declaration for the config button on GPIO0
  pinMode(BTN_BOOT, INPUT_PULLUP);  
}

void buttonLoop() {
  if (digitalRead(BTN_BOOT) == LOW) { // If the button was pressed

    if (btnPressTime == 0) {
      btnPressTime = millis();  // Update the timestamp value
    }

    // If the button is pressed and held for 3 seconds
    if (!wifiCleared && millis() - btnPressTime >= HOLD_TIME) {
      wifiCleared = true; // Toggle the config button status flags

      Serial.println("Reset WiFi config...");

      prefs.begin("wifi", false); // Open the "wifi" namespace for read/write access
      prefs.clear();              // Clear all data
      prefs.end();                // Close the preferences and release resources

      delay(500);
      ESP.restart();  // Restart the ESP32
    }

  } else {
    btnPressTime = 0; // Reset the config button status flags
  }
}
