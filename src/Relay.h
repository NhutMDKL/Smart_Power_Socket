#pragma once

#include <Arduino.h>

#define RELAY1_PIN 5    // GPIO5 - Relay 1
#define RELAY2_PIN 18   // GPIO18 - Relay 2

#define LED1_PIN 21     // GPIO21 - LED 1
#define LED2_PIN 19     // GPIO19 - LED 2

void relayInit();                           // Initialaze relays
void relaySet(uint8_t relay, bool state);   // Control relays and LEDs
bool relayGet(uint8_t relay);               // Read relay state
void relayToggle(uint8_t relay);            // Toggle relay state


