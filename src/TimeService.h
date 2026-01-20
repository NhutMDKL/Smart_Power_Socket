#pragma once

#include <Arduino.h>

void timeInit();        // Initialize NTP and ESP32 system time
void timeLoop();        // Update sysem time every 6 hours
struct tm getNow();     // Get current time from ESP32 system time