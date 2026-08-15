#ifndef PLAYER_H
#define PLAYER_H

#include <stdint.h>

// Player state structure using Q16.16 fixed-point values
typedef struct {
    int32_t x;           // Current X position
    int32_t y;           // Current Y position
    int32_t direction_x; // View direction X component
    int32_t direction_y; // View direction Y component
    int32_t plane_x;     // Camera plane X component (FOV)
    int32_t plane_y;     // Camera plane Y component (FOV)
} Player;

// Initializes the player's position, direction, and camera plane
void player_init(Player *p);

// Moves the player forward along the view direction
void player_move_forward(Player *p, int32_t speed);

// Moves the player backward opposite to the view direction
void player_move_backward(Player *p, int32_t speed);

// Rotates the player's direction and camera plane vectors
void player_rotate(Player *p, int32_t speed);

#endif