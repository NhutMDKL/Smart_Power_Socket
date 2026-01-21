#include "Scheduler.h"
#include "Relay.h"
#include "TimeService.h"

static Schedule schedules[2];
static int lastMinute = -1;

// Initialize scheduler with all schedules disable by default
void schedulerInit() {
  for (int i = 0; i < 2; i++) {
    schedules[i].enable = false;
  }
}

// Update scheduler for the relay 
void schedulerSet(uint8_t relay, Schedule s) {
  schedules[relay - 1] = s;
}

// Get relay schedule
Schedule schedulerGet(uint8_t relay) {
  return schedules[relay - 1];
}

void schedulerLoop() {
  // Get current system time
  struct tm now;
  // Chưa có thời gian => bỏ qua scheduler
  if (!getNow(now)) return;

  // Process only at the start of the new minute, avoid repetition
  if (now.tm_min == lastMinute) return;
  lastMinute = now.tm_min;

  for (int i = 0; i < 2; i++) {
    // Check whether schedules are enabled or disabled
    if (!schedules[i].enable) continue;

    // Check schedules and turn on the relay
    if (now.tm_hour == schedules[i].onHour &&
        now.tm_min  == schedules[i].onMinute) {
      relaySet(i + 1, true);
    }

    // Check schedules and turn off the relay
    if (now.tm_hour == schedules[i].offHour &&
        now.tm_min  == schedules[i].offMinute) {
      relaySet(i + 1, false);
    }
  }
}
