#pragma once

#include <Arduino.h>

void timeInit();                // Initialize NTP and ESP32 system time
void timeLoop();                // Update sysem time every 6 hours
bool getNow(struct tm &out);   // Get current time from ESP32 system time
