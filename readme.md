# BÁO CÁO HỆ NHÚNG

Game Pingpong

## GIỚI THIỆU

- Mô tả lại tính năng, đề bài được yêu cầu.
- Ảnh chụp minh họa:\
  ![Ảnh minh họa](https://soict.hust.edu.vn/wp-content/uploads/logo-soict-hust-1-1024x416.png)

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

- Liệt kê module CPU/dev kit. Ví dụ ESP32 Dev Module, hoặc STM32F429-DISC.
- Liệt kê các kit, module được sử dụng: ví dụ: stm32 cảm biến...

## SƠ ĐỒ SCHEMATIC

_Cho biết cách nối dây, kết nối giữa các linh kiện_
Ví dụ có thể liệt kê dạng bảng
|STM32F429|Module ngoại vi|
|--|--|
|PA0|Nút bấm điều khiển trên board|
|PG13|Led báo trên board|
|PE2|MQ3 SCK|
|PE3|MQ3 SDA|

hoặc dạng ảnh:\
![Sơ đồ Schematic](https://github.com/Hoee1802/MyApp/blob/long/schematic.PNG)

hoặc bất cứ dạng nào thể hiện được cách đấu nối giữa các linh kiện

### TÍCH HỢP HỆ THỐNG

- Mô tả các thành phần phần cứng và vai trò của chúng: máy chủ, máy trạm, thiết bị IoT, MQTT Server, module cảm biến IoT...
- Mô tả các thành phần phần mềm và vai trò của chúng, vị trí nằm trên phần cứng nào: Front-end, Back-end, Worker, Middleware...

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
