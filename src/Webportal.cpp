#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <Preferences.h>
#include "WiFiconfig.h"
#include "Webportal.h"

static WebServer server(80);
static DNSServer dnsServer;
static Preferences prefs;
static String wifiListHTML;

// HTML + CSS (Improved UI)
String page(String body) {
  return R"rawliteral(
  <!DOCTYPE html>
  <html lang="en">
  <head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>ESP32 WiFi Setup</title>

  <style>
  :root{
    --primary:#2563eb;
    --bg:#f3f4f6;
    --card:#ffffff;
    --text:#111827;
    --muted:#6b7280;
    --success:#16a34a;
    --fail:#dc2626;
  }

  *{box-sizing:border-box;}

  body{
    margin:0;
    font-family:system-ui,-apple-system,Segoe UI,Roboto,Ubuntu;
    background:linear-gradient(135deg,#e5e7eb,#f9fafb);
    min-height:100vh;
    display:flex;
    align-items:center;
    justify-content:center;
    color:var(--text);
  }

  .card{
    width:100%;
    max-width:420px;
    background:var(--card);
    padding:24px 22px 26px;
    border-radius:16px;
    box-shadow:0 10px 30px rgba(0,0,0,.12);
  }

  h1{
    margin:0 0 6px;
    font-size:22px;
    text-align:center;
  }

  .subtitle{
    text-align:center;
    font-size:14px;
    color:var(--muted);
    margin-bottom:18px;
  }

  label{
    font-size:14px;
    margin-top:14px;
    display:block;
  }

  select,
  input{
    width:100%;
    margin-top:6px;
    padding:12px 14px;
    border-radius:10px;
    border:1px solid #d1d5db;
    font-size:15px;
    outline:none;
  }

  select:focus,
  input:focus{
    border-color:var(--primary);
  }

  button{
    width:100%;
    margin-top:20px;
    padding:14px;
    border:none;
    border-radius:12px;
    background:var(--primary);
    color:#fff;
    font-size:16px;
    font-weight:600;
    cursor:pointer;
  }

  button:active{
    transform:scale(.98);
  }

  .status{
    text-align:center;
    margin-top:18px;
    font-size:18px;
    font-weight:600;
  }

  .success{color:var(--success);}
  .fail{color:var(--fail);}

  .footer{
    text-align:center;
    margin-top:14px;
    font-size:12px;
    color:var(--muted);
  }
  </style>
  </head>

  <body>
    <div class="card">
      <h1>Smart Power Socket WiFi Setup &#x1F6DC;</h1>
      <div class="subtitle">Configure your WiFi connection</div>
  )rawliteral" + body + R"rawliteral(
      <div class="footer">ESP32 Captive Portal</div>
    </div>
  </body>
  </html>
  )rawliteral";
}

// Check the WiFi RSSI signal
static String getSignalIcon(int rssi) {
  if (rssi >= -50) return " |||| (Wi-Fi Mạnh)"; 
  if (rssi >= -70) return " ||| (Wi-Fi Khá)";  
  if (rssi >= -80) return " || (Wi-Fi Yếu)";
  return " | (Wi-Fi Rất yếu)";            
}

// Scan for WiFi network and display them on web interface
static void scanWiFi() {
  int n = WiFi.scanNetworks();  // Return the scanned WiFi quality
  wifiListHTML = "<select name='ssid' style='padding:5px; width:100%;'>";
  
  for (int i = 0; i < n; i++) {
    int rssi = WiFi.RSSI(i);  // Check the RSSI of the scanned WiFi networks
    String signalStatus = getSignalIcon(rssi); 
    
    wifiListHTML += "<option value='" + WiFi.SSID(i) + "'>";
    // Display the SSID of the scanned WiFi networks
    wifiListHTML += WiFi.SSID(i) + " " + signalStatus; 
    wifiListHTML += "</option>";
  }
  
  wifiListHTML += "</select>";
}

// Send WiFi configuration web interface to the client 
static void handleRoot() {
  scanWiFi();
  String body =
    "<h2>Cấu hình WiFi</h2>"
    "<form action='/connect' method='POST'>"
    + wifiListHTML +
    "<input type='password' name='pass' placeholder='Mật khẩu WiFi'>"
    "<button>Kết nối</button>"
    "</form>";
  server.send(200, "text/html", page(body));
}

// Connect to WiFi using SSID and password via the web interface
static void handleConnect() {
  String ssid = server.arg("ssid"); // Read the SSID via the web interface
  String pass = server.arg("pass"); // Read the password via the web interface

  WiFi.mode(WIFI_AP_STA); // Enable both WiFi AP and STA mode
  WiFi.begin(ssid.c_str(), pass.c_str()); // Connect to WiFi using the provided SSID and password 

  // Wait up to 15 seconds for WiFi connection
  unsigned long t = millis();
  bool ledToggle = false;
  while (WiFi.status() != WL_CONNECTED && millis() - t < 15000) {
    if (ledToggle) {
      digitalWrite(19, HIGH);
      digitalWrite(21, LOW);
    } else {
      digitalWrite(19, LOW);
      digitalWrite(21, HIGH);
    }
    ledToggle = !ledToggle;
    delay(200);
  }
  // Turn off LEDs after connected or timeout
  digitalWrite(19, LOW);
  digitalWrite(21, LOW);

  // If the WiFi connection successfull, write the SSID and password to the "wifi" namespace
  if (WiFi.status() == WL_CONNECTED) {
    prefs.begin("wifi", false);
    prefs.putString("ssid", ssid);
    prefs.putString("pass", pass);
    prefs.end();

    // Send a warning to the client when the connection is successfull
    server.send(200, "text/html",
      page("<p class='success'> &#x2705; Kết nối thành công</p>"
           "<p>Thiết bị sẽ khởi động lại…</p>")
    );

    // Blink both LEDs 5 times to indicate successful connection
    for (int i = 0; i < 5; i++) {
      digitalWrite(19, HIGH);
      digitalWrite(21, HIGH);
      delay(200);
      digitalWrite(19, LOW);
      digitalWrite(21, LOW);
      delay(200);
    }
  } else { // Send a warning only when the connection fails
    server.send(200, "text/html",
      page("<p class='fail'> &#x274C; Kết nối thất bại</p>"
           "<p>Thiết bị sẽ thử lại</p>")
    );
  }

  server.client().stop();   // Ensure the HTTP response is sent
  delay(3000);
  ESP.restart();            // Restart the ESP32
}

// Initialize the WiFi configuration web interface
void webInit() {
  WiFi.softAP(AP_SSID, AP_PASS);  // Enable WiFi AP mode on the ESP32
  // Automatically open the configuration web interface after successfully connecting to WiFi AP mode
  dnsServer.start(DNS_PORT, "*", WiFi.softAPIP()); // captive portal

  // Call the "handRoot" function when the client access IP address
  server.on("/", handleRoot); 
  // Call the "handleConnect" when the client selects Connect on the web interface
  server.on("/connect", HTTP_POST, handleConnect);
  // Call the "handleRoot" function for any request from the client
  server.onNotFound(handleRoot); //(captive portal)

  server.begin(); // Start the web server
}

void webLoop() {
  dnsServer.processNextRequest(); //(captive portal) Automatically open the configuration web interface
  server.handleClient();  // Check for incoming client requests
}
