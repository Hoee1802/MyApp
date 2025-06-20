#include "joystick_task.h"
#include "main.h"
#include "cmsis_os.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_adc.h"
#include "gui/model/Model.hpp"


#include <cstring>
#include <cstdio>

extern ADC_HandleTypeDef hadc1;
extern UART_HandleTypeDef huart1;

extern osMessageQueueId_t joystickQueueHandle;

#define JOY1_X_CHANNEL ADC_CHANNEL_0
#define JOY2_X_CHANNEL ADC_CHANNEL_5

#define JOY1_BUTTON_PIN GPIO_PIN_2
#define JOY2_BUTTON_PIN GPIO_PIN_3
#define JOY_BUTTON_PORT GPIOG

#define JOY_THRESHOLD_LEFT   1000
#define JOY_THRESHOLD_RIGHT  3000

uint32_t tick_counter = 0;


void uart_print(const char* msg)
{
    HAL_UART_Transmit(&huart1, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
}

// ⚠️ Thêm extern "C" ở đây để export hàm cho file C
extern "C" void JoystickTask(void *argument)
{

    uint32_t adc_value1 = 2048;
    uint32_t adc_value2 = 2048;
    ADC_ChannelConfTypeDef sConfig = {0};
    JoystickCommand_t command;

    GPIO_PinState lastJoy1ButtonState = GPIO_PIN_SET;
    GPIO_PinState lastJoy2ButtonState = GPIO_PIN_SET;

    enum Joystick { JOY1, JOY2 };
    Joystick currentJoystick = JOY1;

    tick_counter = 0;


    for (;;) {
    	if (currentJoystick == JOY1) {
			sConfig.Channel = JOY1_X_CHANNEL;
			sConfig.Rank = 1;
			sConfig.SamplingTime = ADC_SAMPLETIME_144CYCLES;
			HAL_ADC_ConfigChannel(&hadc1, &sConfig);

			HAL_ADC_Start(&hadc1);
			HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
			adc_value1 = HAL_ADC_GetValue(&hadc1);
			HAL_ADC_Stop(&hadc1);

			if (adc_value1 < JOY_THRESHOLD_LEFT) {
				command = JOY1_LEFT;
				osMessageQueuePut(joystickQueueHandle, &command, 0, 0);
			} else if (adc_value1 > JOY_THRESHOLD_RIGHT) {
				command = JOY1_RIGHT;
				osMessageQueuePut(joystickQueueHandle, &command, 0, 0);
			}

			GPIO_PinState joy1ButtonState = HAL_GPIO_ReadPin(JOY_BUTTON_PORT, JOY1_BUTTON_PIN);
			if (joy1ButtonState == GPIO_PIN_RESET && lastJoy1ButtonState == GPIO_PIN_SET) {
				command = JOY1_BUTTON;
				osMessageQueuePut(joystickQueueHandle, &command, 0, 0);
			}
			lastJoy1ButtonState = joy1ButtonState;
		} else if (currentJoystick == JOY2) {
			sConfig.Channel = JOY2_X_CHANNEL;
			sConfig.Rank = 1;
			sConfig.SamplingTime = ADC_SAMPLETIME_144CYCLES;
			HAL_ADC_ConfigChannel(&hadc1, &sConfig);

			HAL_ADC_Start(&hadc1);
			HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
			adc_value2 = HAL_ADC_GetValue(&hadc1);
			HAL_ADC_Stop(&hadc1);

			if (adc_value2 < JOY_THRESHOLD_LEFT) {
				command = JOY2_LEFT;
				osMessageQueuePut(joystickQueueHandle, &command, 0, 0);
			} else if (adc_value2 > JOY_THRESHOLD_RIGHT) {
				command = JOY2_RIGHT;
				osMessageQueuePut(joystickQueueHandle, &command, 0, 0);
			}

            GPIO_PinState joy2ButtonState = HAL_GPIO_ReadPin(JOY_BUTTON_PORT, JOY2_BUTTON_PIN);
            if (joy2ButtonState == GPIO_PIN_RESET && lastJoy2ButtonState == GPIO_PIN_SET) {
                command = JOY2_BUTTON;
                osMessageQueuePut(joystickQueueHandle, &command, 0, 0);
            }
            lastJoy2ButtonState = joy2ButtonState;
		}

		tick_counter += 20;
		if (tick_counter >= 500) {
			tick_counter = 0;
			currentJoystick = (currentJoystick == JOY1) ? JOY2 : JOY1;

			char buffer[64];
			sprintf(buffer, "adc1: %lu, adc2: %lu\r\n", adc_value1, adc_value2);
			uart_print(buffer);
		}

        osDelay(10);
    }
}





