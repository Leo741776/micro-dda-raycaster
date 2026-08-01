#include <stdlib.h>

#include "player.h"
#include "map.h"
#include "dda.h"
#include "fixed_point.h"
#include "draw.h"

void update_and_render(Player *p, const int32_t w, const int32_t h)
{
    int32_t camera_x_step = FIXED_DIV(INT_TO_FIXED(2), INT_TO_FIXED(w));
    int32_t camera_x = -Q_SCALE;

    for (int32_t x = 0; x < w; x++) {
        int32_t ray_direction_x = FIXED_ADD(p->direction_x, FIXED_PRODUCT(p->plane_x, camera_x));
        int32_t ray_direction_y = FIXED_ADD(p->direction_y, FIXED_PRODUCT(p->plane_y, camera_x));

        int32_t map_x = FIXED_TO_INT(p->x);
        int32_t map_y = FIXED_TO_INT(p->y);

        int32_t side_distance_x;
        int32_t side_distance_y;

        int32_t delta_distance_x = (ray_direction_x == 0)
            ? Q16_16_MAX
            : abs(FIXED_DIV(Q_SCALE, ray_direction_x));

        int32_t delta_distance_y = (ray_direction_y == 0)
            ? Q16_16_MAX
            : abs(FIXED_DIV(Q_SCALE, ray_direction_y));

        int32_t step_x;
        int32_t step_y;

        int32_t hit = 0;
        int32_t side = 0;

        if (ray_direction_x < 0) {
            step_x = -1;
            side_distance_x = FIXED_PRODUCT(
                FIXED_SUB(p->x, INT_TO_FIXED(map_x)),
                delta_distance_x
            );
        } else {
            step_x = 1;
            side_distance_x = FIXED_PRODUCT(
                FIXED_SUB(FIXED_ADD(INT_TO_FIXED(map_x), Q_SCALE), p->x),
                delta_distance_x
            );
        }

        if (ray_direction_y < 0) {
            step_y = -1;
            side_distance_y = FIXED_PRODUCT(
                FIXED_SUB(p->y, INT_TO_FIXED(map_y)),
                delta_distance_y
            );
        } else {
            step_y = 1;
            side_distance_y = FIXED_PRODUCT(
                FIXED_SUB(FIXED_ADD(INT_TO_FIXED(map_y), Q_SCALE), p->y),
                delta_distance_y
            );
        }

        while (hit == 0) {
            if (side_distance_x < side_distance_y) {
                side_distance_x = FIXED_ADD(side_distance_x, delta_distance_x);
                map_x += step_x;
                side = 0;
            } else {
                side_distance_y = FIXED_ADD(side_distance_y, delta_distance_y);
                map_y += step_y;
                side = 1;
            }

            if (map_is_wall(map_x, map_y)) {
                hit = 1;
            }
        }

        int32_t perpendicular_wall_distance;

        if (side == 0) {
            perpendicular_wall_distance = FIXED_SUB(side_distance_x, delta_distance_x);
        } else {
            perpendicular_wall_distance = FIXED_SUB(side_distance_y, delta_distance_y);
        }

        if (perpendicular_wall_distance <= 0) {
            perpendicular_wall_distance = 1;
        }

        int32_t line_height = FIXED_DIV(h, perpendicular_wall_distance);

        int32_t draw_start = -line_height / 2 + h / 2;
        if (draw_start < 0) {
            draw_start = 0;
        }

        int32_t draw_end = line_height / 2 + h / 2;
        if (draw_end >= h) {
            draw_end = h - 1;
        }

        draw_column(x, draw_start, draw_end, world_map[map_x][map_y], side);

        camera_x = FIXED_ADD(camera_x, camera_x_step);
    }
}