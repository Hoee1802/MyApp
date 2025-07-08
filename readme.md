# BÁO CÁO HỆ NHÚNG

Game Pingpong

## GIỚI THIỆU

- **Đề bài:** Thiết kế và xây dựng hệ thống trò chơi Ping Pong cho 2 người chơi trên nền tảng vi điều khiển STM32F429ZIT6.
- **Tính năng yêu cầu:**
  - Mỗi người chơi điều khiển một vợt thông qua một joystick analog riêng biệt.
  - Hiển thị trạng thái trò chơi (vị trí bóng, vị trí vợt, điểm số) lên màn hình (OLED hoặc LCD).
  - Xử lý va chạm bóng với vợt, tường và cập nhật điểm số.
  - Cho phép bắt đầu/trò chơi lại khi có tín hiệu điều khiển.
  - Đảm bảo tốc độ xử lý mượt mà, không giật lag khi thao tác điều khiển.
  - Hiệu ứng âm thanh (buzzer) khi có điểm.
- **Ý nghĩa:** Dự án giúp sinh viên rèn luyện kỹ năng lập trình nhúng, xử lý ngoại vi (ADC cho joystick, giao tiếp màn hình, ngắt...), thiết kế giao diện điều khiển thực tế, và làm việc nhóm.


- Ảnh chụp minh họa:\
  ![Ảnh minh họa]((https://github.com/Hoee1802/MyApp/blob/main/Schematic.jpg))

## TÁC GIẢ

- Tên nhóm: Chip chip
- Thành viên trong nhóm
  |STT|Họ tên|MSSV|Công việc|
  |--:|--|--|--|
  |1|Vũ Văn Nam|20215623|hiện thị oled, hiệu ứng, và xử lý ngắt|
  |2|Vũ Thị Thanh Hoa|20210356|hiện thị oled, hiệu ứng, và xử lý ngắt|
  |3|Trần Bích Diệp|20215539|hiện thị oled, hiệu ứng, và xử lý ngắt|
  |4|Nguyễn Văn Long|20215610|Vẽ sơ đồ Schematic và viết báo cáo|

## MÔI TRƯỜNG HOẠT ĐỘNG

- **Module CPU / Dev kit sử dụng:**  
  - STM32F429ZIT6 (ARM Cortex-M4 32-bit)

- **Các kit, module, linh kiện sử dụng:**
  - 2 x Joystick analog (điều khiển hai vợt trong game)
  - Buzzer
  - Dây kết nối Dupont

- **Sơ đồ kết nối cơ bản:**

| STM32F429ZIT6 | Joystick 1 | Joystick 2 |   Buzzer   |
|:-------------:|:----------:|:----------:|:----------:|
| 3V3           | VCC        | VCC        |            |
| GND           | GND        | GND        |     GND    |
| PA0           | VRy        |            |            |
| PC3           | VRx        |            |            |
| PG2           | SW         |            |            |
| PA5           |            | VRy        |            |
| PA7           |            | VRx        |            |
| PG3           |            | SW         |            |
| PD12          |            |            |    VCC     |
- **Chức năng từng module:**
  - **STM32F429ZIT6:** Xử lý tín hiệu, đọc dữ liệu joystick, điều khiển logic game ping pong hai người chơi.
  - **Joystick:** Nhập tín hiệu điều khiển từ người chơi (di chuyển vợt).
  - **Buzzer:** Kêu khi người chơi ghi điểm



## SƠ ĐỒ SCHEMATIC

_Cho biết cách nối dây, kết nối giữa các linh kiện_
Ví dụ có thể liệt kê dạng bảng
|STM32F429|Module ngoại vi|
|--|--|
|PA0|Nút bấm điều khiển trên board|
|PE2|MQ3 SCK|
|PE3|MQ3 SDA|

![Sơ đồ Schematic](https://github.com/Hoee1802/MyApp/blob/main/Schematic.jpg)

### TÍCH HỢP HỆ THỐNG

#### **Thành phần phần cứng và vai trò**
- **STM32F429ZIT6**: Vi điều khiển trung tâm, tiếp nhận tín hiệu từ joystick, xử lý thuật toán game và xuất dữ liệu hiển thị.
- **Joystick x2**: Thiết bị nhập liệu, mỗi joystick điều khiển một vợt, cho phép 2 người chơi tương tác với trò chơi.
- **Màn hình OLED/LCD** : Hiển thị trạng thái trò chơi: vị trí bóng, vị trí vợt, điểm số của từng người chơi.
- **Buzzer** : Tạo hiệu ứng âm thanh khi ghi điểm.

#### **Thành phần phần mềm và vai trò**
- **Firmware (chạy trên STM32)**: 
  - Nhận và xử lý tín hiệu analog từ 2 joystick.
  - Thực hiện thuật toán di chuyển bóng, phát hiện va chạm, cập nhật điểm số.
  - Điều khiển giao diện hiển thị (OLED/LCD).
  - Xử lý các hiệu ứng phụ như âm thanh, ánh sáng (nếu có).
- **Giao diện người dùng**:  
  - Được hiển thị trực tiếp trên màn hình nối với STM32.
  - Không sử dụng phần mềm ngoài, mọi thao tác điều khiển và hiển thị được thực hiện toàn bộ trên vi điều khiển và các module ngoại vi.

#### **Lưu ý**
- Hệ thống hoạt động hoàn toàn **độc lập, không cần kết nối mạng, không có máy chủ** hay các thành phần IoT phức tạp.
- Tất cả các chức năng từ nhập liệu, xử lý đến hiển thị đều nằm trên **STM32F429ZIT6** và các module ngoại vi kèm theo.


### ĐẶC TẢ HÀM

- Giải thích một số hàm quan trọng: ý nghĩa của hàm, tham số vào, ra

  ```C
     /**
      *  Hàm tính ...
      *  @param  x  Tham số
      *  @param  y  Tham số
      */
     void abc(int x, int y = 2);
  ```

### KẾT QUẢ

- Các ảnh chụp với caption giải thích.
- Hoặc video sản phẩm
