#include <WiFi.h>
#include <Preferences.h>
#include "Wifimanager.h"

// Return the WiFi connection status
bool wifiConnectFromStorage() {
  // Read the SSID and password from the "wifi" namespace
  Preferences prefs;
  prefs.begin("wifi", true);
  String ssid = prefs.getString("ssid", "");
  String pass = prefs.getString("pass", "");
  prefs.end();

  // Return false if the SSID is empty
  if (ssid == "") return false;

  WiFi.mode(WIFI_STA);  // Enable WiFi STA mode 
  WiFi.begin(ssid.c_str(), pass.c_str()); // Connect to WiFi network

  // Wait to 10 seconds for the WiFi connection 
  unsigned long t = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - t < 10000) {
    delay(300);
  }

  // Return true if the WiFi connection is successful; otherwise, return false
  return WiFi.status() == WL_CONNECTED;
}
