#include "TimeService.h"
#include <time.h>

#define NTP_SERVER "pool.ntp.org"     // Get time from NTP server
#define GMT_OFFSET_SEC (7 * 3600)     // Time zone offset for VietNam (UTC+7)
#define DAYLIGHT_OFFSET 0             // No daylight saving time in VietNam
#define RTC_SYNC_INTERVAL (6 * 3600)  // Synchronize ESP32 RTC with NTP time every 6 hours

static time_t lastSync = 0;   // Last NTP sysc timestamp (time_t)

void timeInit() {
  // Synchronize system time (ESP32 RTC) with NTP server
  configTime(GMT_OFFSET_SEC, DAYLIGHT_OFFSET, NTP_SERVER);

  // Get current time from ESP32 system time (time_t)
  time(&lastSync);
}

void timeLoop() {
  time_t now;
  time(&now); // Get current time from ESP32 system time (time_t)

  // Update the ESP32 system time using NTP every 6 hours
  if (now - lastSync > RTC_SYNC_INTERVAL) {
    configTime(GMT_OFFSET_SEC, DAYLIGHT_OFFSET, NTP_SERVER);
    lastSync = now;
  }
}

// Get current date and time as broken-down time (struct tm)
struct tm getNow() {
  struct tm timeinfo;
  getLocalTime(&timeinfo);
  return timeinfo;
}
