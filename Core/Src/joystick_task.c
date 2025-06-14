#include "joystick_task.h"
#include "main.h"
#include "cmsis_os.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_adc.h"

// Nếu chưa có extern ADC_HandleTypeDef:
extern ADC_HandleTypeDef hadc1;

// Các channel và chân GPIO
#define JOY1_X_CHANNEL ADC_CHANNEL_0
#define JOY2_X_CHANNEL ADC_CHANNEL_2

#define JOY1_SW_PIN    GPIO_PIN_1
#define JOY2_SW_PIN    GPIO_PIN_3
#define JOY_GPIO_PORT  GPIOA

// Ngưỡng để xác định trái/phải
#define JOY_THRESHOLD_LEFT   1000
#define JOY_THRESHOLD_RIGHT  3000

// Dummy actions – thay bằng logic thật trong game
void movePlayer1Left(void)   { /* TODO: gọi presenter/model hoặc queue */ }
void movePlayer1Right(void)  { }
void player1Action(void)     { }

void movePlayer2Left(void)   { }
void movePlayer2Right(void)  { }
void player2Action(void)     { }

void JoystickTask(void *argument)
{
    uint8_t joy1_btn_last = 0, joy2_btn_last = 0;
    uint32_t adc_value;
    ADC_ChannelConfTypeDef sConfig = {0};

    for (;;) {
        // === JOYSTICK 1 ===
        sConfig.Channel = JOY1_X_CHANNEL;
        sConfig.Rank = 1;  // Thay vì ADC_REGULAR_RANK_1
        sConfig.SamplingTime = ADC_SAMPLETIME_15CYCLES;
        HAL_ADC_ConfigChannel(&hadc1, &sConfig);

        HAL_ADC_Start(&hadc1);
        HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
        adc_value = HAL_ADC_GetValue(&hadc1);
        HAL_ADC_Stop(&hadc1);

        if (adc_value < JOY_THRESHOLD_LEFT) {
            movePlayer1Left();
        } else if (adc_value > JOY_THRESHOLD_RIGHT) {
            movePlayer1Right();
        }

        if (HAL_GPIO_ReadPin(JOY_GPIO_PORT, JOY1_SW_PIN) == GPIO_PIN_RESET && !joy1_btn_last) {
            player1Action();
            joy1_btn_last = 1;
        } else if (HAL_GPIO_ReadPin(JOY_GPIO_PORT, JOY1_SW_PIN) == GPIO_PIN_SET) {
            joy1_btn_last = 0;
        }

        // === JOYSTICK 2 ===
        sConfig.Channel = JOY2_X_CHANNEL;
        HAL_ADC_ConfigChannel(&hadc1, &sConfig);

        HAL_ADC_Start(&hadc1);
        HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
        adc_value = HAL_ADC_GetValue(&hadc1);
        HAL_ADC_Stop(&hadc1);

        if (adc_value < JOY_THRESHOLD_LEFT) {
            movePlayer2Left();
        } else if (adc_value > JOY_THRESHOLD_RIGHT) {
            movePlayer2Right();
        }

        if (HAL_GPIO_ReadPin(JOY_GPIO_PORT, JOY2_SW_PIN) == GPIO_PIN_RESET && !joy2_btn_last) {
            player2Action();
            joy2_btn_last = 1;
        } else if (HAL_GPIO_ReadPin(JOY_GPIO_PORT, JOY2_SW_PIN) == GPIO_PIN_SET) {
            joy2_btn_last = 0;
        }

        osDelay(50); // delay 50ms mỗi lần đọc
    }
}
