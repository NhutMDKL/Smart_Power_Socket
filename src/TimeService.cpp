#include "TimeService.h"
#include <time.h>

#define NTP_SERVER "pool.ntp.org"     // Get time from NTP server
#define GMT_OFFSET_SEC (7 * 3600)     // Time zone offset for VietNam (UTC+7)
#define DAYLIGHT_OFFSET 0             // No daylight saving time in VietNam
#define RTC_SYNC_INTERVAL (6 * 3600)  // Synchronize ESP32 RTC with NTP time every 6 hours

static time_t lastSync = 0;   // Last NTP sysc timestamp (time_t)

// Lưu thời gian hợp lệ cuối cùng
static struct tm lastValidTime;
static bool hasValidTime = false;

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
bool getNow(struct tm &out) {
  // timeout = 0  => KHÔNG block
  if (getLocalTime(&out, 0)) {
    lastValidTime = out;     // cache lại thời gian hợp lệ
    hasValidTime = true;
    return true;
  }

  // Nếu chưa có NTP nhưng đã từng có time trước đó
  if (hasValidTime) {
    out = lastValidTime;    // dùng lại time cũ (fail-safe)
  }

  return false;
}
