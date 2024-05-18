#ifndef FILLBARDATA_H
#define FILLBARDATA_H

#include "vec2.h"

typedef struct {
    bool initialized;

    vec2_t pos;
    vec2_t size;
    u32 fill_color;
    u32 bg_color;
    float max_value;

} fill_bar_data_t;

#endif