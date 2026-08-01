/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32c0xx_hal.h"
#include "stm32c0xx_nucleo.h"
#include <stdio.h>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define RCC_OSCX_IN_Pin       GPIO_PIN_14
#define RCC_OSCX_IN_GPIO_Port GPIOC
#define RCC_OSCX_OUT_Pin      GPIO_PIN_15
#define RCC_OSCX_OUT_GPIO_Port GPIOC
#define RCC_OSC_IN_Pin        GPIO_PIN_0
#define RCC_OSC_IN_GPIO_Port  GPIOF
#define RCC_OSC_OUT_Pin       GPIO_PIN_1
#define RCC_OSC_OUT_GPIO_Port GPIOF
#define ST7735_RST_Pin        GPIO_PIN_0
#define ST7735_RST_GPIO_Port  GPIOB
#define ST7735_DC_Pin         GPIO_PIN_1
#define ST7735_DC_GPIO_Port   GPIOB
#define ST7735_CS_Pin         GPIO_PIN_2
#define ST7735_CS_GPIO_Port   GPIOB
#define DEBUG_SWDIO_Pin       GPIO_PIN_13
#define DEBUG_SWDIO_GPIO_Port GPIOA
#define DEBUG_SWCLK_Pin       GPIO_PIN_14
#define DEBUG_SWCLK_GPIO_Port GPIOA
#define BTN_UP_Pin            GPIO_PIN_0
#define BTN_UP_GPIO_Port      GPIOD
#define BTN_DOWN_Pin          GPIO_PIN_1
#define BTN_DOWN_GPIO_Port    GPIOD
#define BTN_LEFT_Pin          GPIO_PIN_2
#define BTN_LEFT_GPIO_Port    GPIOD
#define BTN_RIGHT_Pin         GPIO_PIN_3
#define BTN_RIGHT_GPIO_Port   GPIOD

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */