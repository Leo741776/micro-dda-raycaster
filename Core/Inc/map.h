#ifndef MAP_H
#define MAP_H

#include <stdint.h>
#include <stdbool.h>

// Grid dimensions for the world map
#define MAP_WIDTH  24
#define MAP_HEIGHT 24

// External reference to the 2D grid representing the level layout
extern const uint8_t world_map[MAP_WIDTH][MAP_HEIGHT];

static inline bool map_is_wall(int32_t x, int32_t y)
{
    if (x >= MAP_WIDTH || x < 0 || y >= MAP_HEIGHT || y < 0) {
        return true;
    }

    return world_map[x][y] > 0;
}

#endif