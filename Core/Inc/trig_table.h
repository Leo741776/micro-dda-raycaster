#ifndef TRIG_TABLE_H
#define TRIG_TABLE_H

#include <stdint.h>

// Total degree entries in the lookup table representing a full circle
#define TABLE_SIZE 360

// External precomputed sine table using fixed-point values
extern const int32_t sin_table[TABLE_SIZE];

// Looks up the sine value for a given angle in degrees, normalizing bounds
static inline int32_t lookup_sin(int16_t deg)
{
    while (deg < 0) {
        deg += TABLE_SIZE;
    }

    while (deg >= TABLE_SIZE) {
        deg -= TABLE_SIZE;
    }

    return sin_table[deg];
}

// Looks up the cosine value by shifting the angle by 90 degrees
static inline int32_t lookup_cos(int16_t deg)
{
    return lookup_sin(deg + 90);
}

#endif