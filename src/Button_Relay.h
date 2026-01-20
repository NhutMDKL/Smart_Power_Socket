#pragma once

#include <Arduino.h>

#define RELAY_BTN1_PIN 17   // GPIO17 - relay 1 control button
#define RELAY_BTN2_PIN 16   // GPIO16 - relay 2 control button

void relayButtonInit(); // Initialaze relay buttons
void relayButtonLoop(); // Check relay button states

