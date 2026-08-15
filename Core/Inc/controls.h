#ifndef CONTROLS_H
#define CONTROLS_H

#include "main.h"
#include <stdint.h>

#define BTN_UP_PORT    GPIOD
#define BTN_UP_PIN     GPIO_PIN_0

#define BTN_DOWN_PORT  GPIOD
#define BTN_DOWN_PIN   GPIO_PIN_1

#define BTN_LEFT_PORT  GPIOD
#define BTN_LEFT_PIN   GPIO_PIN_2

#define BTN_RIGHT_PORT GPIOD
#define BTN_RIGHT_PIN  GPIO_PIN_3

static inline uint8_t is_button_held(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	return (HAL_GPIO_ReadPin(GPIOx, GPIO_Pin) == GPIO_PIN_RESET);
}

#endif