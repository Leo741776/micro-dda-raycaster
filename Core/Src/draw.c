#include "draw.h"
#include "st7735.h"

void draw_column(int32_t x, int32_t draw_start, int32_t draw_end, int32_t wall_type, int32_t side)
{
    int32_t clamped_start = draw_start;
    int32_t clamped_end = draw_end;

    // Clamp drawing boundaries to screen limits
    if (clamped_start < 0) {
        clamped_start = 0;
    }

    if (clamped_end >= 128) {
        clamped_end = 127;
    }

    // Draw the ceiling segment from the top of the screen to the wall
    if (clamped_start > 0) {
        ST7735_draw_column((uint8_t)x, 0, (uint8_t)(clamped_start - 1), COLOR_CEILING);
    }

    uint16_t color;

    // Select standard or darkened color variant depending on wall side hit
    if (side == 0) {
        switch (wall_type) {
        case 1:
            color = COLOR_VIOLET;
            break;
        case 2:
            color = COLOR_GOLD;
            break;
        case 3:
            color = COLOR_EMERALD;
            break;
        case 4:
            color = COLOR_WHITE;
            break;
        default:
            color = COLOR_YELLOW;
            break;
        }
    } else {
        switch (wall_type) {
        case 1:
            color = COLOR_VIOLET_DARK;
            break;
        case 2:
            color = COLOR_GOLD_DARK;
            break;
        case 3:
            color = COLOR_EMERALD_DARK;
            break;
        case 4:
            color = COLOR_WHITE_DARK;
            break;
        default:
            color = COLOR_YELLOW_DARK;
            break;
        }
    }

    // Draw the vertical wall slice
    ST7735_draw_column((uint8_t)x, (uint8_t)clamped_start, (uint8_t)clamped_end, color);

    // Draw the floor segment from the bottom of the wall to the screen bottom
    if (clamped_end < 127) {
        ST7735_draw_column((uint8_t)x, (uint8_t)(clamped_end + 1), 127, COLOR_FLOOR);
    }
}