#include "draw.h"
#include "display.h"

void draw_column(int32_t x, int32_t draw_start, int32_t draw_end, int32_t wall_type, int32_t side)
{
    int32_t clamped_start = draw_start;
    int32_t clamped_end = draw_end;

    if (clamped_start < 0) {
        clamped_start = 0;
    }

    if (clamped_end >= 128) {
        clamped_end = 127;
    }

    if (clamped_start > 0) {
        st7735_draw_column((uint8_t)x, 0, (uint8_t)(clamped_start - 1), COLOR_CEILING);
    }

    uint16_t color;

    if (side == 0) {
        switch (wall_type) {
        case 1:
            color = COLOR_RED;
            break;
        case 2:
            color = COLOR_GREEN;
            break;
        case 3:
            color = COLOR_BLUE;
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
            color = COLOR_RED_DARK;
            break;
        case 2:
            color = COLOR_GREEN_DARK;
            break;
        case 3:
            color = COLOR_BLUE_DARK;
            break;
        case 4:
            color = COLOR_WHITE_DARK;
            break;
        default:
            color = COLOR_YELLOW_DARK;
            break;
        }
    }

    st7735_draw_column((uint8_t)x, (uint8_t)clamped_start, (uint8_t)clamped_end, color);

    if (clamped_end < 127) {
        st7735_draw_column((uint8_t)x, (uint8_t)(clamped_end + 1), 127, COLOR_FLOOR);
    }
}