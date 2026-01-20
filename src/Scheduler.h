#pragma once

#include <Arduino.h>

// Schedules sructure for the relay
struct Schedule {
  uint8_t onHour;
  uint8_t onMinute;
  uint8_t offHour;
  uint8_t offMinute;
  bool enable;
};

void schedulerInit(); // Initialize scheduler for the relay
void schedulerLoop(); // Control the relay based on schadules

void schedulerSet(uint8_t relay, Schedule s); // Update scheduler for the relay
Schedule schedulerGet(uint8_t relay);         // Get relay schedule


