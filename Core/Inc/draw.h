#ifndef DRAW_H
#define DRAW_H

#include <stdint.h>

// 16-bit RGB565 color definitions
#define COLOR_VIOLET        0x9B39
#define COLOR_GOLD          0xFEA0
#define COLOR_EMERALD       0x562F
#define COLOR_WHITE         0xFFFF
#define COLOR_YELLOW        0xFFE0

// Darker variants of standard colors for shading or distance fog effects
#define COLOR_VIOLET_DARK   0x49AC
#define COLOR_GOLD_DARK     0x7B40
#define COLOR_EMERALD_DARK  0x2B27
#define COLOR_WHITE_DARK    0x7BEF
#define COLOR_YELLOW_DARK   0x7BE0

// Environment colors for the raycasting viewport
#define COLOR_CEILING       0x2104
#define COLOR_FLOOR         0x39E7

void draw_column(int32_t x, int32_t draw_start, int32_t draw_end, int32_t wall_type, int32_t side);

#endif