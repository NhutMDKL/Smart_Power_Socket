#include "WebUI.h"
#include <WebServer.h>
#include <ESPmDNS.h>
#include <WiFi.h>
#include <Preferences.h>
#include "Relay.h"
#include "TimeService.h"
#include "Scheduler.h"

WebServer server(80);

// Web interface relay control
String htmlPage() {
  return R"rawliteral(
  <!DOCTYPE html>
  <html lang="vi">
  <head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="data:image/svg+xml,
  <svg xmlns='http://www.w3.org/2000/svg' viewBox='0 0 100 100'>
  <text y='0.9em' font-size='90'>&#x1F4A1;</text>
  </svg>">
  <title>ESP32 Relay Timer</title>

  <style>
  body{
    font-family:Arial,Helvetica,sans-serif;
    background:#eef2f7;
    text-align:center;
    margin:0;
    padding:0;
  }

  h2{
    background:#2c3e50;
    color:white;
    margin:0;
    padding:15px;
  }

  .container{
    max-width:420px;
    margin:auto;
    padding:10px;
  }

  .card{
    background:#ffffff;
    padding:18px;
    margin:15px 0;
    border-radius:12px;
    box-shadow:0 4px 10px rgba(0,0,0,0.08);
  }

  .card h3{
    margin-top:0;
    color:#2c3e50;
  }

  button{
    width:100%;
    padding:12px;
    font-size:16px;
    border:none;
    border-radius:8px;
    cursor:pointer;
    margin-top:10px;
  }

  .btn-r1{background:#27ae60;color:white;}
  .btn-r2{background:#f39c12;color:white;}
  .btn-save{background:#2980b9;color:white;}
  .btn-wifi{background:#e74c3c;color:white;}

  button:active{opacity:0.8;}

  label{
    display:block;
    text-align:left;
    margin-top:10px;
    font-size:14px;
    color:#555;
  }

  input,select{
    width:100%;
    padding:8px;
    font-size:15px;
    margin-top:4px;
    border-radius:6px;
    border:1px solid #ccc;
  }

  .note{
    font-size:13px;
    color:#777;
    margin-top:8px;
  }

  .switch{
    position:relative;
    display:inline-block;
    width:54px;
    height:28px;
  }

  .switch input{
    opacity:0;
    width:0;
    height:0;
  }

  .slider{
    position:absolute;
    cursor:pointer;
    top:0; left:0; right:0; bottom:0;
    background-color:#ccc;
    transition:.4s;
    border-radius:34px;
  }

  .slider:before{
    position:absolute;
    content:"";
    height:22px;
    width:22px;
    left:3px;
    bottom:3px;
    background:white;
    transition:.4s;
    border-radius:50%;
  }

  input:checked + .slider{
    background-color:#27ae60;
  }

  input:checked + .slider:before{
    transform:translateX(26px);
  }

  </style>
  </head>

  <body>

  <h2>Điều Khiển Relay ESP32</h2>

  <div class="container">

  <!-- RELAY 1 -->
  <div class="card">
    <h3>&#x1F50C; Relay 1</h3>
    <button class="btn-r1" onclick="fetch('/toggle?relay=1')">
      BẬT / TẮT THỦ CÔNG
    </button>
  </div>

  <!-- RELAY 2 -->
  <div class="card">
    <h3>&#x1F50C; Relay 2</h3>
    <button class="btn-r2" onclick="fetch('/toggle?relay=2')">
      BẬT / TẮT THỦ CÔNG
    </button>
  </div>

  <!-- TIMER -->
  <div class="card">
    <h3>&#x23F0; Hẹn Giờ Relay</h3>

    <form action="/set_timer">
      
      <label>Chọn relay</label>
      <select name="relay">
        <option value="1">Relay 1</option>
        <option value="2">Relay 2</option>
      </select>

      <label>Bật lúc</label>
      <input type="time" name="on">

      <label>Tắt lúc</label>
      <input type="time" name="off">

      <label>Trạng thái lịch</label>
      <div class="form-row">
        <!-- input thật gửi cho ESP32 -->
        <input type="hidden" name="en" id="enValue" value="1">

        <!-- công tắc -->
        <label class="switch">
          <input type="checkbox" checked
            onchange="document.getElementById('enValue').value = this.checked ? 1 : 0;">
          <span class="slider"></span>
        </label>
      </div>

      <button class="btn-save" type="submit">
        &#x1F4BE; Lưu Hẹn Giờ
      </button>

      <div class="note">
        * Khi hẹn giờ được kích hoạt, relay sẽ tự động bật/tắt theo thời gian đã đặt.
      </div>

    </form>
  </div>

  <!-- WIFI -->
  <div class="card" style="background:#fff0f0">
    <h3>&#x1F6E0; Cấu Hình WiFi</h3>
    <button class="btn-wifi"
    onclick="if(confirm('Xoá cấu hình WiFi và khởi động lại ESP32?')) fetch('/wifi_reset')">
      Xoá cấu hình WiFi
    </button>
    <div class="note">
      Thiết bị sẽ khởi động lại và vào chế độ cấu hình WiFi.
    </div>
  </div>

  </div>
  </body>
  </html>
  )rawliteral";
}

// Send relay control web interface to the client
void handleRoot() {
  server.send(200, "text/html", htmlPage());
}

void handleToggle() {
  // Read relay control parameter
  uint8_t relay = server.arg("relay").toInt();
  relayToggle(relay); // Toggle the relay based on control parameter
  server.send(200, "text/plain", "OK"); // Send success notification to client
}

void handleSetTimer() {
  // Read relay control parameter
  uint8_t relay = server.arg("relay").toInt();

  String onTime  = server.arg("on");   // Get relay turn-on time
  String offTime = server.arg("off");  // Get relay turn-off time

  // Write relay turn-on and turn-off times to the scheduler
  Schedule s;
  s.onHour     = onTime.substring(0, 2).toInt();
  s.onMinute   = onTime.substring(3, 5).toInt();
  s.offHour    = offTime.substring(0, 2).toInt();
  s.offMinute  = offTime.substring(3, 5).toInt();
  s.enable     = server.arg("en").toInt();  

  schedulerSet(relay, s); // Update scheduler for relay

  // Display scheduler success message and redirect after 1 second
  server.send(200, "text/html",
    "<!DOCTYPE html>"
    "<html>"
    "<head>"
      "<meta charset='UTF-8'>"
      "<meta http-equiv='refresh' content='1;url=/'/>"
      "<style>"
        "body{font-family:Arial;text-align:center;background:#f2f2f2;margin-top:80px;}"
        ".box{display:inline-block;padding:20px 30px;"
        "background:#ffffff;border-radius:10px;"
        "box-shadow:0 0 10px rgba(0,0,0,0.15);}"
        "h3{color:#2e7d32;margin:0;}"
        "p{color:#555;font-size:14px;}"
      "</style>"
    "</head>"
    "<body>"
      "<div class='box'>"
        "<h3>&#x2705; Đã lưu hẹn giờ</h3>"
        "<p>Đang quay về trang chính...</p>"
      "</div>"
    "</body>"
    "</html>"
  );
}

// Clear all sroted WiFi SSID and password preferences
void handleWifiReset() {
  Preferences prefs;
  prefs.begin("wifi", false); // Open the "wifi" preferences namespace
  prefs.clear();              // Clear all stored preferences
  prefs.end();                // Close the preferences

  delay(1000);
  ESP.restart();  // Restart the ESP32
}

void webUIInit() {
  MDNS.begin("O-cam-thong-minh"); // Enable mDNS with "http://O-cam-thong-minh.local"

  // Call handleRoot when the client accesses the device via IP or mDNS
  server.on("/", handleRoot); 
  // Call handleToggle when the client control the relay via the web interface
  server.on("/toggle", handleToggle); 
   // Call handleSetTimer when the client schedules relay control
  server.on("/set_timer", handleSetTimer);
   // Call handleWifiReset when the client wants to reset wifi
  server.on("/wifi_reset", handleWifiReset);

  server.begin();
}

// Handle client request
void webUILoop() {
  server.handleClient();
}

