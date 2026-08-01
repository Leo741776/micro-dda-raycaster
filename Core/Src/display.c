#include "display.h"
#include "main.h"

extern SPI_HandleTypeDef hspi1;

#define ST7735_X_OFFSET 2
#define ST7735_Y_OFFSET 1

#define ST7735_SWRESET 0x01
#define ST7735_SLPOUT  0x11
#define ST7735_FRMCTR1 0xB1
#define ST7735_PWCTR1  0xC0
#define ST7735_PWCTR2  0xC1
#define ST7735_PWCTR3  0xC2
#define ST7735_VMCTR1  0xC5
#define ST7735_INVON   0x21
#define ST7735_MADCTL  0x36
#define ST7735_COLMOD  0x3A
#define ST7735_GMCTRP1 0xE0
#define ST7735_GMCTRN1 0xE1
#define ST7735_NORON   0x13
#define ST7735_DISPON  0x29
#define ST7735_CASET   0x2A
#define ST7735_RASET   0x2B
#define ST7735_RAMWR   0x2C

void st7735_init(void)
{
    HAL_GPIO_WritePin(ST7735_RST_GPIO_Port, ST7735_RST_Pin, GPIO_PIN_RESET);
    HAL_Delay(5);
    HAL_GPIO_WritePin(ST7735_RST_GPIO_Port, ST7735_RST_Pin, GPIO_PIN_SET);
    HAL_Delay(150);

    HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_SET);
    HAL_Delay(10);

    st7735_write_command(ST7735_SWRESET);
    HAL_Delay(120);

    st7735_write_command(ST7735_SLPOUT);
    HAL_Delay(120);

    st7735_write_command(ST7735_FRMCTR1);
    uint8_t frmctr1[] = {0x01, 0x2C, 0x2D};
    st7735_write_data(frmctr1, 3);

    st7735_write_command(ST7735_PWCTR1);
    uint8_t pwctr1[] = {0xA2, 0x02, 0x84};
    st7735_write_data(pwctr1, 3);

    st7735_write_command(ST7735_PWCTR2);
    uint8_t pwctr2 = 0xC5;
    st7735_write_data(&pwctr2, 1);

    st7735_write_command(ST7735_PWCTR3);
    uint8_t pwctr3[] = {0x0A, 0x00};
    st7735_write_data(pwctr3, 2);

    st7735_write_command(ST7735_VMCTR1);
    uint8_t vmctr1 = 0x0E;
    st7735_write_data(&vmctr1, 1);

    st7735_write_command(ST7735_INVON);

    st7735_write_command(ST7735_MADCTL);
    uint8_t madctl = 0xC0;
    st7735_write_data(&madctl, 1);

    st7735_write_command(ST7735_COLMOD);
    uint8_t color_mode = 0x05;
    st7735_write_data(&color_mode, 1);

    st7735_write_command(ST7735_GMCTRP1);
    uint8_t gmctrp1[] = {
        0x02, 0x1C, 0x07, 0x12, 0x37, 0x32, 0x29, 0x2D,
        0x29, 0x25, 0x2B, 0x39, 0x00, 0x01, 0x03, 0x10
    };
    st7735_write_data(gmctrp1, 16);

    st7735_write_command(ST7735_GMCTRN1);
    uint8_t gmctrn1[] = {
        0x03, 0x1D, 0x07, 0x06, 0x2E, 0x2C, 0x29, 0x2D,
        0x2E, 0x2E, 0x37, 0x3F, 0x00, 0x00, 0x02, 0x10
    };
    st7735_write_data(gmctrn1, 16);

    st7735_write_command(ST7735_NORON);
    HAL_Delay(10);

    st7735_write_command(ST7735_DISPON);
    HAL_Delay(10);
}

void st7735_write_command(uint8_t cmd)
{
    HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(ST7735_DC_GPIO_Port, ST7735_DC_Pin, GPIO_PIN_RESET);

    HAL_SPI_Transmit(&hspi1, &cmd, 1, HAL_MAX_DELAY);

    HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_SET);
}

void st7735_write_data(const uint8_t *data, uint16_t size)
{
    HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(ST7735_DC_GPIO_Port, ST7735_DC_Pin, GPIO_PIN_SET);

    HAL_SPI_Transmit(&hspi1, (uint8_t *)data, size, HAL_MAX_DELAY);

    HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_SET);
}

void st7735_draw_column(uint8_t x, uint8_t y_start, uint8_t y_end, uint16_t color)
{
    if (y_start > y_end) {
        return;
    }

    uint8_t adj_x = x + ST7735_X_OFFSET;
    uint8_t adj_y_start = y_start + ST7735_Y_OFFSET;
    uint8_t adj_y_end = y_end + ST7735_Y_OFFSET;

    st7735_write_command(ST7735_CASET);
    uint8_t column_data[] = {0x00, adj_x, 0x00, adj_x};
    st7735_write_data(column_data, 4);

    st7735_write_command(ST7735_RASET);
    uint8_t row_data[] = {0x00, adj_y_start, 0x00, adj_y_end};
    st7735_write_data(row_data, 4);

    st7735_write_command(ST7735_RAMWR);

    uint16_t line_height = (y_end - y_start) + 1;
    if (line_height > 128) {
        line_height = 128;
    }

    uint8_t col_buffer[256];
    uint8_t hi = (color >> 8) & 0xFF;
    uint8_t lo = color & 0xFF;

    for (uint16_t i = 0; i < line_height; i++) {
        col_buffer[i * 2] = hi;
        col_buffer[(i * 2) + 1] = lo;
    }

    st7735_write_data(col_buffer, line_height * 2);
}