#include "joystick_task.h"          // Header tùy chỉnh cho định nghĩa tác vụ joystick (ví dụ: JoystickCommand_t)
#include "main.h"                  // Header chính cho thư viện HAL của STM32 và cấu hình ngoại vi
#include "cmsis_os.h"              // API CMSIS-RTOS cho tích hợp FreeRTOS
#include "stm32f4xx_hal.h"         // Thư viện HAL cho STM32F4 để truy cập ngoại vi
#include "stm32f4xx_hal_adc.h"     // Mô-đun HAL ADC cho chuyển đổi analog sang số
#include "gui/model/Model.hpp"      // Header C++ cho mô hình GUI (có thể liên quan đến logic ứng dụng)

// #include <cstring>              // Bị comment: Thư viện chuỗi chuẩn của C (không sử dụng)
// #include <cstdio>               // Bị comment: Thư viện I/O chuẩn của C (không sử dụng)

extern ADC_HandleTypeDef hadc1;    // Handle ADC bên ngoài cho ngoại vi ADC1
extern UART_HandleTypeDef huart1;  // Handle UART bên ngoài cho UART1 (không sử dụng trong mã này)

extern osMessageQueueId_t joystickQueueHandle; // Handle hàng đợi tin nhắn FreeRTOS cho các lệnh joystick

// Định nghĩa kênh ADC cho trục X của joystick
#define JOY1_X_CHANNEL ADC_CHANNEL_0  // Kênh ADC cho trục X của Joystick 1
#define JOY2_X_CHANNEL ADC_CHANNEL_5  // Kênh ADC cho trục X của Joystick 2

// Định nghĩa chân GPIO và cổng cho nút bấm của joystick
#define JOY1_BUTTON_PIN GPIO_PIN_2    // Chân GPIO cho nút bấm của Joystick 1
#define JOY2_BUTTON_PIN GPIO_PIN_3    // Chân GPIO cho nút bấm của Joystick 2
#define JOY_BUTTON_PORT GPIOG         // Cổng GPIO cho cả hai nút bấm joystick

// Định nghĩa ngưỡng ADC để phát hiện chuyển động của joystick
#define JOY_THRESHOLD_LEFT   1000     // Ngưỡng ADC cho chuyển động sang trái (~0.25 dải 12-bit ADC)
#define JOY_THRESHOLD_RIGHT  3000     // Ngưỡng ADC cho chuyển động sang phải (~0.75 dải 12-bit ADC)

uint32_t tick_counter = 0;            // Biến đếm thời gian toàn cục để chuyển đổi giữa các joystick

// ⚠️ extern "C" đảm bảo liên kết kiểu C cho hàm tác vụ để tương thích với FreeRTOS (dựa trên C)
extern "C" void JoystickTask(void *argument)
{
    // Khởi tạo giá trị ADC ở giữa dải (2048 cho ADC 12-bit, vị trí trung tâm)
    uint32_t adc_value1 = 2048;       // Giá trị ADC cho trục X của Joystick 1
    uint32_t adc_value2 = 2048;       // Giá trị ADC cho trục X của Joystick 2

    // Cấu trúc cấu hình kênh ADC
    ADC_ChannelConfTypeDef sConfig = {0}; // Khởi tạo tất cả trường bằng 0

    // Biến lệnh joystick cho các tin nhắn hàng đợi (ví dụ: JOY1_LEFT, JOY1_RIGHT, v.v.)
    JoystickCommand_t command;

    // Theo dõi trạng thái trước đó của nút bấm để phát hiện thay đổi trạng thái (chống dội phím)
    GPIO_PinState lastJoy1ButtonState = GPIO_PIN_SET; // Trạng thái ban đầu của nút Joystick 1 (không nhấn)
    GPIO_PinState lastJoy2ButtonState = GPIO_PIN_SET; // Trạng thái ban đầu của nút Joystick 2 (không nhấn)

    // Enum để chuyển đổi giữa JOY1 và JOY2 khi lấy mẫu ADC theo thời gian
    enum Joystick { JOY1, JOY2 };
    Joystick currentJoystick = JOY1;  // Bắt đầu với Joystick 1

    tick_counter = 0;                // Khởi tạo bộ đếm thời gian

    // Vòng lặp vô hạn để xử lý joystick liên tục
    for (;;) {
        // Xử lý Joystick 1
        if (currentJoystick == JOY1) {
            // Cấu hình ADC cho kênh trục X của Joystick 1
            sConfig.Channel = JOY1_X_CHANNEL;        // Chọn kênh ADC
            sConfig.Rank = 1;                       // Đặt thứ tự kênh thường
            sConfig.SamplingTime = ADC_SAMPLETIME_144CYCLES; // Đặt thời gian lấy mẫu
            HAL_ADC_ConfigChannel(&hadc1, &sConfig); // Áp dụng cấu hình cho ADC1

            // Bắt đầu chuyển đổi ADC và đọc giá trị
            HAL_ADC_Start(&hadc1);                  // Khởi động ADC
            HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY); // Chờ hoàn thành chuyển đổi
            adc_value1 = HAL_ADC_GetValue(&hadc1); // Lấy giá trị ADC
            HAL_ADC_Stop(&hadc1);             // Dừng ADC

            // Kiểm tra chuyển động của joystick dựa trên ngưỡng
            if (adc_value1 < JOY_THRESHOLD_LEFT) {
                command = JOY1_LEFT;               // Đặt lệnh cho chuyển động sang trái
                osMessageQueuePut(joystickQueueHandle, &command, 0, 0); // Gửi vào hàng đợi
            } else if (adc_value1 > JOY_THRESHOLD_RIGHT) {
                command = JOY1_RIGHT;              // Đặt lệnh cho chuyển động sang phải
                osMessageQueuePut(joystickQueueHandle, &command, 0, 0); // Gửi vào hàng đợi
            }

            // Đọc trạng thái nút bấm Joystick 1 và phát hiện nhấn (cạnh xuống)
            GPIO_PinState joy1ButtonState = HAL_GPIO_ReadPin(JOY_BUTTON_PORT, JOY1_BUTTON_PIN);
            if (joy1ButtonState == GPIO_PIN_RESET && lastJoy1ButtonState == GPIO_PIN_SET) {
                command = JOY1_BUTTON;             // Đặt lệnh cho nút bấm
                osMessageQueuePut(joystickQueueHandle, &command, 0, 0); // Gửi vào hàng đợi
            }
            lastJoy1ButtonState = joy1ButtonState; // Cập nhật trạng thái nút bấm trước đó

        // Xử lý Joystick 2
        } else if (currentJoystick == JOY2) {
            // Cấu hình ADC cho kênh trục X của Joystick 2
            sConfig.Channel = JOY2_X_CHANNEL;       // Chọn kênh ADC
            sConfig.Rank = 1;                      // Đặt thứ tự kênh thường
            sConfig.SamplingTime = ADC_SAMPLETIME_144CYCLES; // Đặt thời gian lấy mẫu
            HAL_ADC_ConfigChannel(&hadc1, &sConfig); // Áp dụng cấu hình cho ADC1

            // Bắt đầu chuyển đổi ADC và đọc giá trị
            HAL_ADC_Start(&hadc1);                  // Khởi động ADC
            HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY); // Chờ hoàn thành chuyển đổi
            adc_value2 = HAL_ADC_GetValue(&hadc1);  // Lấy giá trị ADC
            HAL_ADC_Stop(&hadc1);                // Dừng ADC

            // Kiểm tra chuyển động của joystick dựa trên ngưỡng
            if (adc_value2 < JOY_THRESHOLD_LEFT) {
                command = JOY2_LEFT;               // Đặt lệnh cho chuyển động sang trái
                osMessageQueuePut(joystickQueueHandle, &command, 0, 0); // Gửi vào hàng đợi
            } else if (adc_value2 > JOY_THRESHOLD_RIGHT) {
                command = JOY2_RIGHT;              // Đặt lệnh cho chuyển động sang phải
                osMessageQueuePut(joystickQueueHandle, &command, 0, 0); // Gửi vào hàng đợi
            }

            // Đọc trạng thái nút bấm Joystick 2 và phát hiện nhấn (cạnh xuống)
            GPIO_PinState joy2ButtonState = HAL_GPIO_ReadPin(JOY_BUTTON_PORT, JOY2_BUTTON_PIN);
            if (joy2ButtonState == GPIO_PIN_RESET && lastJoy2ButtonState == GPIO_PIN_SET) {
                command = JOY2_BUTTON;             // Đặt lệnh cho nút bấm
                osMessageQueuePut(joystickQueueHandle, &command, 0, 0); // Gửi vào hàng đợi
            }
            lastJoy2ButtonState = joy2ButtonState; // Cập nhật trạng thái nút bấm trước đó
        }

        // Cập nhật bộ đếm thời gian để chuyển đổi joystick
        tick_counter += 20;                    // Tăng 20ms (gần đúng)
        if (tick_counter >= 500) {             // Chuyển joystick sau mỗi 500ms
            tick_counter = 0;                  // Đặt lại bộ đếm
            currentJoystick = (currentJoystick == JOY1) ? JOY2 : JOY1; // Chuyển đổi joystick
        }

        osDelay(10);                           // Tạm dừng 10ms để kiểm soát tần số tác vụ
    }
}
