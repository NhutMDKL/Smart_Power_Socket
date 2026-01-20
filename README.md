# 🔌 Smart Power Socket – Ổ Cắm Thông Minh Dùng ESP32

## 1. Giới thiệu
Dự án **Smart Power Socket** là một ổ cắm thông minh sử dụng **ESP32**, cho phép người dùng điều khiển thiết bị điện một cách linh hoạt và an toàn thông qua **nút nhấn vật lý** hoặc **giao diện Web** trong cùng mạng Wi‑Fi.

Các chức năng chính của dự án tập trung vào những nhu cầu cơ bản nhưng thiết thực, phù hợp cho **học tập, nghiên cứu hệ thống nhúng** và làm nền tảng cho các ứng dụng IoT gia đình.

> ⚠️ **Lưu ý an toàn:** Mặc dù relay có khả năng chịu dòng lên đến 10A, dự án **chỉ khuyến nghị tải tối đa 200W** để đảm bảo độ bền linh kiện và an toàn khi vận hành.

---

## 2. Tính năng chính

- 🔘 **Điều khiển thủ công** relay bằng nút nhấn vật lý
- 🌐 **Điều khiển từ xa trong LAN** qua Web Server tích hợp trên ESP32
- ⏱️ **Hẹn giờ bật/tắt relay** theo lịch định sẵn (Scheduler)
- 📡 Hoạt động độc lập, chỉ yêu cầu cùng mạng Wi‑Fi

### Giới hạn hiện tại
- Giao diện Web **chưa phản hồi trạng thái relay theo thời gian thực**
- Mỗi relay **chỉ hỗ trợ một lịch hẹn tại một thời điểm**
- Lịch hẹn **chưa được khôi phục sau khi mất điện**

---

## 3. Demo dự án

### ▶️ Video demo trên YouTube
📺 **Xem video tại đây:**  
👉 https://www.youtube.com/your-demo-link

---

### 🔘 Demo 1 – Bật/tắt thủ công bằng nút nhấn

![Manual Switch Demo](docs/gif/manual_switch.gif)

---

### 🌐 Demo 2 – Bật/tắt relay bằng giao diện Web

![Web Control Demo](docs/gif/web_control.gif)

---

## 4. Phần cứng sử dụng

- ESP32 Dev Module
- Module Relay AC
- Nút nhấn vật lý
- LED báo trạng thái
- Ổ cắm AC
- Adapter nguồn AC–DC (220VAC → 5VDC)

---

## 5. Sơ đồ mạch nguyên lý

Sơ đồ mạch dưới đây mô tả cách kết nối các thành phần chính trong hệ thống:

![Schematic Diagram](docs/images/schematic.png)

---

## 6. Nguyên lý hoạt động (tóm tắt)

- ESP32 khởi động và kết nối Wi‑Fi
- Web Server nội bộ được khởi tạo để nhận lệnh điều khiển
- Scheduler liên tục kiểm tra thời gian để quyết định bật/tắt relay
- Nút nhấn vật lý cho phép điều khiển trực tiếp ngay cả khi không truy cập Web

---

## 7. Tác giả

👤 **Minh Nhựt**  
📍 Việt Nam  
📘 Lĩnh vực: Embedded Systems – IoT – Firmware

---

⭐ Nếu bạn thấy dự án hữu ích, hãy **Star repository** để ủng hộ và theo dõi các phiên bản tiếp theo!

