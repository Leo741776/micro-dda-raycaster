#ifndef TRIG_TABLE_H
#define TRIG_TABLE_H

#include <stdint.h>

#define TABLE_SIZE 360

extern const int32_t sin_table[TABLE_SIZE];

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

static inline int32_t lookup_cos(int16_t deg)
{
    return lookup_sin(deg + 90);
}

#endif