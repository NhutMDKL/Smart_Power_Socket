#include <Arduino.h>
#include "esp_task_wdt.h"
#include "Wifimanager.h"
#include "Webportal.h"
#include "Button.h"
#include "Relay.h"
#include "Button_Relay.h"
#include "WebUI.h"
#include "TimeService.h"
#include "Scheduler.h"

void setup() {
  Serial.begin(115200);
  esp_task_wdt_init(120, true); // Initialize WDT with 120s timeout
  esp_task_wdt_add(NULL);       // Add loop stask to WDT

  buttonInit();       // Initialize the WiFi clear button 
  relayInit();        // Initialize the relays
  relayButtonInit();  // Initialize the physical relay control buttons
  schedulerInit();    // Initialize the relay control scheduler

  if (wifiConnectFromStorage()) { // Successfully connected to the WiFi network
    Serial.println("WiFi OK");
    timeInit();   // Get current time from the ESP32 system time
    webUIInit();  // Send the relay control web interface to the client
  } else { // WiFi connection failed
    Serial.println("No WiFi, Enable AP mode");
    webInit(); // Send the WiFi configuration web interface to the client
  }
}

void loop() {
  esp_task_wdt_reset(); // Feed WDT
  buttonLoop();         // Check the Wifi configuration clear button
  relayButtonLoop();    // Check the relay control buttons
  webLoop();            // WiFi configuration web interface
  webUILoop();          // Relay control web interface
  schedulerLoop();      // Check the relay control scheduler 
  timeLoop();           // Update the ESP32 system time every 6 hours
}
