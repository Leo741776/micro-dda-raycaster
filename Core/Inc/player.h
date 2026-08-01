#ifndef PLAYER_H
#define PLAYER_H

#include <stdint.h>

typedef struct {
    int32_t x;
    int32_t y;
    int32_t direction_x;
    int32_t direction_y;
    int32_t plane_x;
    int32_t plane_y;
} Player;

void player_init(Player *p);
void player_move_forward(Player *p, int32_t speed);
void player_move_backward(Player *p, int32_t speed);
void player_rotate(Player *p, int32_t speed);

#endif