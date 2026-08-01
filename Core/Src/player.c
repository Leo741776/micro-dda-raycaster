#include "player.h"
#include "map.h"
#include "fixed_point.h"
#include "trig_table.h"

void player_init(Player *p)
{
    p->x = FLOAT_TO_FIXED(10.0f);
    p->y = FLOAT_TO_FIXED(10.0f);
    p->direction_x = FLOAT_TO_FIXED(1.0f);
    p->direction_y = FLOAT_TO_FIXED(0.0f);
    p->plane_x = FLOAT_TO_FIXED(0.0f);
    p->plane_y = FLOAT_TO_FIXED(0.66f);
}

void player_move_forward(Player *p, int32_t speed)
{
    int32_t step_x = FIXED_PRODUCT(p->direction_x, speed);
    int32_t step_y = FIXED_PRODUCT(p->direction_y, speed);

    int32_t next_fixed_x = FIXED_ADD(p->x, step_x);
    int32_t next_fixed_y = FIXED_ADD(p->y, step_y);

    if (!map_is_wall(FIXED_TO_INT(next_fixed_x), FIXED_TO_INT(p->y))) {
        p->x = next_fixed_x;
    }

    if (!map_is_wall(FIXED_TO_INT(p->x), FIXED_TO_INT(next_fixed_y))) {
        p->y = next_fixed_y;
    }
}

void player_move_backward(Player *p, int32_t speed)
{
    int32_t step_x = FIXED_PRODUCT(p->direction_x, speed);
    int32_t step_y = FIXED_PRODUCT(p->direction_y, speed);

    int32_t next_fixed_x = FIXED_SUB(p->x, step_x);
    int32_t next_fixed_y = FIXED_SUB(p->y, step_y);

    if (!map_is_wall(FIXED_TO_INT(next_fixed_x), FIXED_TO_INT(p->y))) {
        p->x = next_fixed_x;
    }

    if (!map_is_wall(FIXED_TO_INT(p->x), FIXED_TO_INT(next_fixed_y))) {
        p->y = next_fixed_y;
    }
}

void player_rotate(Player *p, int32_t angle_delta)
{
    int32_t cos_val = lookup_cos(angle_delta);
    int32_t sin_val = lookup_sin(angle_delta);

    int32_t old_dir_x = p->direction_x;

    p->direction_x = FIXED_SUB(
        FIXED_PRODUCT(p->direction_x, cos_val),
        FIXED_PRODUCT(p->direction_y, sin_val)
    );

    p->direction_y = FIXED_ADD(
        FIXED_PRODUCT(old_dir_x, sin_val),
        FIXED_PRODUCT(p->direction_y, cos_val)
    );

    int32_t old_plane_x = p->plane_x;

    p->plane_x = FIXED_SUB(
        FIXED_PRODUCT(p->plane_x, cos_val),
        FIXED_PRODUCT(p->plane_y, sin_val)
    );

    p->plane_y = FIXED_ADD(
        FIXED_PRODUCT(old_plane_x, sin_val),
        FIXED_PRODUCT(p->plane_y, cos_val)
    );
}