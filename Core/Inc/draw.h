#ifndef DRAW_H
#define DRAW_H

#include <stdint.h>

#define COLOR_RED         0xF800
#define COLOR_GREEN       0x07E0
#define COLOR_BLUE        0x001F
#define COLOR_WHITE       0xFFFF
#define COLOR_YELLOW      0xFFE0

#define COLOR_RED_DARK    0x7800
#define COLOR_GREEN_DARK  0x03E0
#define COLOR_BLUE_DARK   0x000F
#define COLOR_WHITE_DARK  0x7BEF
#define COLOR_YELLOW_DARK 0x7BE0

#define COLOR_CEILING     0x2104
#define COLOR_FLOOR       0x39E7

void draw_column(int32_t x, int32_t draw_start, int32_t draw_end, int32_t wall_type, int32_t side);

#endif