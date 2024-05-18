#ifndef BUTTONDATA_H
#define BUTTONDATA_H

#include <citro2d.h>

#include "vec2.h"

typedef struct {
    vec2_t pos;
    vec2_t size;
    u32 sprite_idx;
    u32 color;

} button_data_t;

#endif