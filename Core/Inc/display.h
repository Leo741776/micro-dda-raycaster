#ifndef DISPLAY_H
#define DISPLAY_H

#include "main.h"
#include <stdint.h>

#define ST7735_CS_GPIO_Port   GPIOB
#define ST7735_CS_Pin         GPIO_PIN_2

#define ST7735_DC_GPIO_Port   GPIOB
#define ST7735_DC_Pin         GPIO_PIN_1

#define ST7735_RST_GPIO_Port  GPIOB
#define ST7735_RST_Pin        GPIO_PIN_0

void st7735_init(void);
void st7735_write_command(uint8_t cmd);
void st7735_write_data(const uint8_t *data, uint16_t size);

void st7735_draw_column(uint8_t x, uint8_t y_start, uint8_t y_end, uint16_t color);

#endif