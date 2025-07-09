#include "joystick_task.h"          // Header tùy chỉnh cho định nghĩa tác vụ joystick
#include "main.h"                  // Header chính cho thư viện HAL của STM32
#include "cmsis_os.h"              // API CMSIS-RTOS cho FreeRTOS
#include "stm32f4xx_hal.h"         // Thư viện HAL cho STM32F4
#include "stm32f4xx_hal_adc.h"     // Mô-đun HAL ADC
#include "gui/model/Model.hpp"      // Header C++ cho mô hình GUI
#include <cstring>
#include <cstdio>

extern ADC_HandleTypeDef hadc1;    // Handle ADC cho ADC1
extern UART_HandleTypeDef huart1;  // Handle UART cho UART1
extern osMessageQueueId_t joystickQueueHandle; // Handle hàng đợi FreeRTOS

// Định nghĩa kênh ADC
#define JOY1_X_CHANNEL ADC_CHANNEL_13 // Kênh ADC cho trục X của Joystick 1
#define JOY1_Y_CHANNEL ADC_CHANNEL_0  // Kênh ADC cho trục Y của Joystick 1
#define JOY2_X_CHANNEL ADC_CHANNEL_7 // Kênh ADC cho trục X của Joystick 2
#define JOY2_Y_CHANNEL ADC_CHANNEL_5  // Kênh ADC cho trục Y của Joystick 2

#define JOY1_BUTTON_PIN GPIO_PIN_2    // Chân GPIO cho nút bấm Joystick 1
#define JOY2_BUTTON_PIN GPIO_PIN_3    // Chân GPIO cho nút bấm Joystick 2
#define JOY_BUTTON_PORT GPIOG         // Cổng GPIO cho nút bấm

#define JOY_THRESHOLD_LEFT   1000     // Ngưỡng cho chuyển động trái
#define JOY_THRESHOLD_RIGHT  3000     // Ngưỡng cho chuyển động phải


//uint32_t tick_counter = 0;            // Biến đếm thời gian


extern "C" void JoystickTask(void *argument)
{
    // Khởi tạo giá trị ADC ở giữa dải (2048 cho ADC 12-bit)
    uint32_t adc_values[4] = {2048, 2048, 2048, 2048}; // mảng lưu giá trị ADC: [JOY1_X, JOY1_Y, JOY2_X, JOY2_Y]


    ADC_ChannelConfTypeDef sConfig = {0};//đặt các trường về 0
    uint8_t channel_rank = 1;

    // Cấu hình tất cả 4 kênh cho ADC1 ở chế độ Scan
    uint32_t channels[4] = {JOY1_X_CHANNEL, JOY1_Y_CHANNEL, JOY2_X_CHANNEL, JOY2_Y_CHANNEL};
    for (int i = 0; i < 4; i++) {
        sConfig.Channel = channels[i];
        sConfig.Rank = channel_rank++;
        sConfig.SamplingTime = ADC_SAMPLETIME_144CYCLES;//quét tuần tự 4 kênh, mỗi kênh lấy mẫu trong 144 chu kỳ
        HAL_ADC_ConfigChannel(&hadc1, &sConfig);
    }

    // Cấu hình ADC1 ở chế độ Scan
    hadc1.Instance->CR1 |= ADC_CR1_SCAN; // Bật chế độ Scan
    hadc1.Instance->SQR1 &= ~(ADC_SQR1_L); // Xóa số lượng kênh
    hadc1.Instance->SQR1 |= (3 << 20); // Thiết lập 4 kênh (L = 3, vì đếm từ 0)

    // Biến lệnh joystick
    JoystickCommand_t command;

    // Theo dõi trạng thái nút bấm
    GPIO_PinState lastJoy1ButtonState = GPIO_PIN_SET;
    GPIO_PinState lastJoy2ButtonState = GPIO_PIN_SET;

    for (;;) {
        // Bắt đầu quét tất cả các kênh
        HAL_ADC_Start(&hadc1);

        // Đọc lần lượt giá trị từ 4 kênh
        for (int i = 0; i < 4; i++) {
            HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);//polling giá trị adc
            adc_values[i] = HAL_ADC_GetValue(&hadc1);
        }
        HAL_ADC_Stop(&hadc1);

        // Xử lý Joystick 1
        // Trục X
        if (adc_values[1] < JOY_THRESHOLD_LEFT) {
            command = JOY1_LEFT;
            osMessageQueuePut(joystickQueueHandle, &command, 0, 0);//lưu vào hàng đợi command
        } else if (adc_values[1] > JOY_THRESHOLD_RIGHT) {
            command = JOY1_RIGHT;
            osMessageQueuePut(joystickQueueHandle, &command, 0, 0);
        }

        // Trục Y
        if (adc_values[0] < JOY_THRESHOLD_LEFT) {
            command = JOY1_UP;
            osMessageQueuePut(joystickQueueHandle, &command, 0, 0);
        }else if (adc_values[0] > JOY_THRESHOLD_RIGHT) {
			command = JOY1_DOWN;
			osMessageQueuePut(joystickQueueHandle, &command, 0, 0);
		}

        // Xử lý nút bấm Joystick 1
        GPIO_PinState joy1ButtonState = HAL_GPIO_ReadPin(JOY_BUTTON_PORT, JOY1_BUTTON_PIN);
        if (joy1ButtonState == GPIO_PIN_RESET && lastJoy1ButtonState == GPIO_PIN_SET) {
            command = JOY1_BUTTON;
            osMessageQueuePut(joystickQueueHandle, &command, 0, 0);
        }
        lastJoy1ButtonState = joy1ButtonState;

        // Xử lý Joystick 2
        // Trục X
        if (adc_values[3] < JOY_THRESHOLD_LEFT) {
            command = JOY2_LEFT;
            osMessageQueuePut(joystickQueueHandle, &command, 0, 0);
        } else if (adc_values[3] > JOY_THRESHOLD_RIGHT) {
            command = JOY2_RIGHT;
            osMessageQueuePut(joystickQueueHandle, &command, 0, 0);
        }

        // Trục Y
        if (adc_values[2] < JOY_THRESHOLD_LEFT) {
            command = JOY2_UP;
            osMessageQueuePut(joystickQueueHandle, &command, 0, 0);
        }
        else if (adc_values[2] > JOY_THRESHOLD_RIGHT) {
		   command = JOY2_DOWN;
		   osMessageQueuePut(joystickQueueHandle, &command, 0, 0);
	   }
        // Xử lý nút bấm Joystick 2
        GPIO_PinState joy2ButtonState = HAL_GPIO_ReadPin(JOY_BUTTON_PORT, JOY2_BUTTON_PIN);
        if (joy2ButtonState == GPIO_PIN_RESET && lastJoy2ButtonState == GPIO_PIN_SET) {
            command = JOY2_BUTTON;
            osMessageQueuePut(joystickQueueHandle, &command, 0, 0);
        }
        lastJoy2ButtonState = joy2ButtonState;

        osDelay(10); // Tạm dừng 10ms
    }
}
