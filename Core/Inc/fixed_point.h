#ifndef FIXED_POINT_H
#define FIXED_POINT_H

#include <stdint.h>

// Fixed-point format configuration (Q16.16)
#define Q_SHIFT 16
#define Q_SCALE (1 << Q_SHIFT)

// Conversion macros between floating-point, integer, and fixed-point representations
#define FLOAT_TO_FIXED(x) ((int32_t)((x) * Q_SCALE))
#define FIXED_TO_FLOAT(x) ((float)(x) / Q_SCALE)
#define FIXED_TO_INT(x)   ((x) >> Q_SHIFT)
#define INT_TO_FIXED(x)   ((int32_t)(x) << Q_SHIFT)

// Basic addition and subtraction (standard arithmetic applies directly)
#define FIXED_ADD(x, y) ((x) + (y))
#define FIXED_SUB(x, y) ((x) - (y))

// Advanced fixed-point multiplication and division with 64-bit casting to prevent overflow
#define FIXED_PRODUCT(x, y) ((int32_t)(((int64_t)(x) * (y)) >> Q_SHIFT))
#define FIXED_DIV(x, y)     ((int32_t)(((int64_t)(x) << Q_SHIFT) / (y)))

// Maximum positive value representable in Q16.16 format
#define Q16_16_MAX 0x7FFFFFFF

#endif