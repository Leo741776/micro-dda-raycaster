#include "st7735.h"

int16_t _width;   ///< Display width as modified by current rotation
int16_t _height;  ///< Display height as modified by current rotation
int16_t cursor_x; ///< x location to start print()ing text
int16_t cursor_y; ///< y location to start print()ing text
uint8_t rotation; ///< Display rotation (0 thru 3)
uint8_t _colstart; ///< Some displays need this changed to offset
uint8_t _rowstart; ///< Some displays need this changed to offset
uint8_t _xstart;
uint8_t _ystart;

const uint8_t
init_cmds1[] = { // Init for 7735R, part 1 (red or green tab)
    15, // 15 commands in list:
    ST7735_SWRESET,
    DELAY, //   1: Software reset, 0 args, w/delay
    150, //     150 ms delay
    ST7735_SLPOUT,
    DELAY, //   2: Out of sleep mode, 0 args, w/delay
    255, //     500 ms delay
    ST7735_FRMCTR1,
    3, //   3: Frame rate ctrl - normal mode, 3 args:
    0x01,
    0x2C,
    0x2D, //     Rate = fosc/(1x2+40) * (LINE+2C+2D)
    ST7735_FRMCTR2,
    3, //   4: Frame rate control - idle mode, 3 args:
    0x01,
    0x2C,
    0x2D, //     Rate = fosc/(1x2+40) * (LINE+2C+2D)
    ST7735_FRMCTR3,
    6, //   5: Frame rate ctrl - partial mode, 6 args:
    0x01,
    0x2C,
    0x2D, //     Dot inversion mode
    0x01,
    0x2C,
    0x2D, //     Line inversion mode
    ST7735_INVCTR,
    1, //   6: Display inversion ctrl, 1 arg, no delay:
    0x07, //     No inversion
    ST7735_PWCTR1,
    3, //   7: Power control, 3 args, no delay:
    0xA2,
    0x02, //     -4.6V
    0x84, //     AUTO mode
    ST7735_PWCTR2,
    1, //   8: Power control, 1 arg, no delay:
    0xC5, //     VGH25 = 2.4C VGSEL = -10 VGH = 3 * AVDD
    ST7735_PWCTR3,
    2, //   9: Power control, 2 args, no delay:
    0x0A, //     Opamp current small
    0x00, //     Boost frequency
    ST7735_PWCTR4,
    2, // 10: Power control, 2 args, no delay:
    0x8A, //     BCLK/2, Opamp current small & Medium low
    0x2A,
    ST7735_PWCTR5,
    2, // 11: Power control, 2 args, no delay:
    0x8A,
    0xEE,
    ST7735_VMCTR1,
    1, // 12: Power control, 1 arg, no delay:
    0x0E,
    ST7735_INVOFF,
    0, // 13: Don't invert display, no args, no delay
    ST7735_COLMOD,
    1, // 15: set color mode, 1 arg, no delay:
    0x05
  }, //     16-bit color

  #if defined(ST7735_IS_128X128)
init_cmds2[] = {
    2,
    ST7735_CASET,
    4,
    0x00,
    0x00,
    0x00,
    0x7F, // XEND = 127 for 128x128
    ST7735_RASET,
    4,
    0x00,
    0x00,
    0x00,
    0x7F
  }, // YEND = 127 for 128x128

  #elif defined(ST7735_IS_160X128)
init_cmds2[] = {
    2,
    ST7735_CASET,
    4,
    0x00,
    0x00,
    0x00,
    0x9F, // XEND = 159 for 160x128 landscape width!
    ST7735_RASET,
    4,
    0x00,
    0x00,
    0x00,
    0x7F
  }, // YEND = 127 for height
  #endif

#ifdef ST7735_IS_160X80
init_cmds2[] = { // Init for 7735S, part 2 (160x80 display)
    3, //   3 commands in list:
    ST7735_CASET,
    4, //   1: Column addr set, 4 args, no delay:
    0x00,
    0x00, //     XSTART = 0
    0x00,
    0x4F, //     XEND = 79
    ST7735_RASET,
    4, //   2: Row addr set, 4 args, no delay:
    0x00,
    0x00, //     XSTART = 0
    0x00,
    0x9F, //     XEND = 159
    ST7735_INVON,
    0
  }, //   3: Invert colors
  #endif

init_cmds3[] = { // Init for 7735R, part 3 (red or green tab)
  4, //   4 commands in list:
  ST7735_GMCTRP1,
  16, //   1: Magical unicorn dust, 16 args, no delay:
  0x02,
  0x1c,
  0x07,
  0x12,
  0x37,
  0x32,
  0x29,
  0x2d,
  0x29,
  0x25,
  0x2B,
  0x39,
  0x00,
  0x01,
  0x03,
  0x10,
  ST7735_GMCTRN1,
  16, //   2: Sparkles and rainbows, 16 args, no delay:
  0x03,
  0x1d,
  0x07,
  0x06,
  0x2E,
  0x2C,
  0x29,
  0x2D,
  0x2E,
  0x2E,
  0x37,
  0x3F,
  0x00,
  0x00,
  0x02,
  0x10,
  ST7735_NORON,
  DELAY, //   3: Normal display on, no args, w/delay
  10, //     10 ms delay
  ST7735_DISPON,
  DELAY, //   4: Main screen turn on, no args w/delay
  100
}; //     100 ms delay

void ST7735_Select(void) {
    HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_RESET);
}

void ST7735_Unselect(void) {
    HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_SET);
}

void ST7735_Reset(void) {
    HAL_GPIO_WritePin(RST_PORT, RST_PIN, GPIO_PIN_RESET);
    HAL_Delay(5);
    HAL_GPIO_WritePin(RST_PORT, RST_PIN, GPIO_PIN_SET);
}

void ST7735_WriteCommand(uint8_t cmd) {
    HAL_GPIO_WritePin(DC_PORT, DC_PIN, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&ST7735_SPI_PORT, &cmd, sizeof(cmd), HAL_MAX_DELAY);
}

void ST7735_WriteData(uint8_t *buff, size_t buff_size) {
    HAL_GPIO_WritePin(DC_PORT, DC_PIN, GPIO_PIN_SET);
    HAL_SPI_Transmit(&ST7735_SPI_PORT, buff, buff_size, HAL_MAX_DELAY);
}

void DisplayInit(const uint8_t *addr) {
    uint8_t numCommands, numArgs;
    uint16_t ms;

    numCommands = *addr++;
    while (numCommands--) {
        uint8_t cmd = *addr++;
        ST7735_WriteCommand(cmd);

        numArgs = *addr++;
        // If high bit set, delay follows args
        ms = numArgs & DELAY;
        numArgs &= ~DELAY;
        if (numArgs) {
            ST7735_WriteData((uint8_t *)addr, numArgs);
            addr += numArgs;
        }

        if (ms) {
            ms = *addr++;
            if (ms == 255) ms = 500;
            HAL_Delay(ms);
        }
    }
}

void ST7735_SetAddressWindow(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {
    // Column address set
    ST7735_WriteCommand(ST7735_CASET);
    uint8_t data[] = {
        0x00,
        x0 + _xstart,
        0x00,
        x1 + _xstart
    };
    ST7735_WriteData(data, sizeof(data));

    // Row address set
    ST7735_WriteCommand(ST7735_RASET);
    data[1] = y0 + _ystart;
    data[3] = y1 + _ystart;
    ST7735_WriteData(data, sizeof(data));

    // Prepare to write pixel data to RAM
    ST7735_WriteCommand(ST7735_RAMWR);
}

void ST7735_Init(uint8_t rotation) {
    ST7735_Select();
    ST7735_Reset();
    DisplayInit(init_cmds1);
    DisplayInit(init_cmds2);
    DisplayInit(init_cmds3);
    #if ST7735_IS_160X80
    _colstart = 24;
    _rowstart = 0;
    
    uint8_t data = 0xC0;
    ST7735_Select();
    ST7735_WriteCommand(ST7735_MADCTL);
    ST7735_WriteData(&data, 1);
    ST7735_Unselect();

    #elif ST7735_IS_128X128
    _colstart = 2;
    _rowstart = 3;
    #else
    _colstart = 0;
    _rowstart = 0;
    #endif
    ST7735_SetRotation(rotation);
    ST7735_Unselect();
}

void ST7735_SetRotation(uint8_t m) {
    uint8_t madctl = 0;

    rotation = m % 4; // Ensure rotation stays within 0 to 3

    switch (rotation) {
    case 0:
        #if ST7735_IS_160X80
        madctl = ST7735_MADCTL_MX | ST7735_MADCTL_MY | ST7735_MADCTL_BGR;
        #else
        madctl = ST7735_MADCTL_MX | ST7735_MADCTL_MY | ST7735_MADCTL_RGB;
        _height = ST7735_HEIGHT;
        _width = ST7735_WIDTH;
        _xstart = _colstart;
        _ystart = _rowstart;
        #endif
        break;
    case 1:
        #if ST7735_IS_160X80
        madctl = ST7735_MADCTL_MY | ST7735_MADCTL_MV | ST7735_MADCTL_BGR;
        #else
        madctl = ST7735_MADCTL_MY | ST7735_MADCTL_MV | ST7735_MADCTL_RGB;
        _width = ST7735_HEIGHT;
        _height = ST7735_WIDTH;
        _ystart = _colstart;
        _xstart = _rowstart;
        #endif
        break;
    case 2:
        #if ST7735_IS_160X80
        madctl = ST7735_MADCTL_BGR;
        #else
        madctl = ST7735_MADCTL_RGB;
        _height = ST7735_HEIGHT;
        _width = ST7735_WIDTH;
        _xstart = _colstart;
        _ystart = _rowstart;
        #endif
        break;
    case 3:
        #if ST7735_IS_160X80
        madctl = ST7735_MADCTL_MX | ST7735_MADCTL_MV | ST7735_MADCTL_BGR;
        #else
        madctl = ST7735_MADCTL_MX | ST7735_MADCTL_MV | ST7735_MADCTL_RGB;
        _width = ST7735_HEIGHT;
        _height = ST7735_WIDTH;
        _ystart = _colstart;
        _xstart = _rowstart;
        #endif
        break;
    }
    ST7735_Select();
    ST7735_WriteCommand(ST7735_MADCTL);
    ST7735_WriteData(&madctl, 1);
    ST7735_Unselect();
}

void ST7735_DrawPixel(uint16_t x, uint16_t y, uint16_t color) {
    if ((x >= _width) || (y >= _height))
        return;

    ST7735_Select();
    ST7735_SetAddressWindow(x, y, x + 1, y + 1);
    
    uint8_t data[] = {
        color >> 8,
        color & 0xFF
    };
    ST7735_WriteData(data, sizeof(data));
    ST7735_Unselect();
}

void ST7735_FillRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) {
    if ((x >= _width) || (y >= _height)) return;
    if ((x + w - 1) >= _width) w = _width - x;
    if ((y + h - 1) >= _height) h = _height - y;

    ST7735_Select();
    ST7735_SetAddressWindow(x, y, x + w - 1, y + h - 1);

    uint8_t data[] = {
        color >> 8,
        color & 0xFF
    };
    
    HAL_GPIO_WritePin(DC_PORT, DC_PIN, GPIO_PIN_SET);
    for (y = h; y > 0; y--) {
        for (x = w; x > 0; x--) {
            HAL_SPI_Transmit(&ST7735_SPI_PORT, data, sizeof(data), HAL_MAX_DELAY);
        }
    }

    ST7735_Unselect();
}

void ST7735_DrawImage(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const uint16_t *data) {
    if ((x >= _width) || (y >= _height)) return;
    if ((x + w - 1) >= _width) return;
    if ((y + h - 1) >= _height) return;

    ST7735_Select();
    ST7735_SetAddressWindow(x, y, x + w - 1, y + h - 1);
    ST7735_WriteData((uint8_t *)data, sizeof(uint16_t) * w * h);
    ST7735_Unselect();
}

void ST7735_InvertColors(bool invert) {
    ST7735_Select();
    ST7735_WriteCommand(invert ? ST7735_INVON : ST7735_INVOFF);
    ST7735_Unselect();
}

void ST7735_draw_column(uint8_t x, uint8_t y_start, uint8_t y_end, uint16_t color) {
    if (y_start > y_end) {
        return;
    }

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

    ST7735_Select();
    ST7735_SetAddressWindow(x, y_start, x, y_end);
    ST7735_WriteData(col_buffer, line_height * 2);
    ST7735_Unselect();
}