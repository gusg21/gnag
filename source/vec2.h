#ifndef VEC2_H
#define VEC2_H

#include <3ds.h>
#include "mathf.h"

typedef struct {
    float x, y;
} vec2_t;

typedef struct {
    s32 x, y;
} vec2i_t;

vec2_t Vec2_Lerp(vec2_t a, vec2_t b, float t);

#endif  // VEC2_H