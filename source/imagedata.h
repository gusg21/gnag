#ifndef IMAGEDATA_H
#define IMAGEDATA_H

#include <citro2d.h>

#include "vec2.h"

typedef enum {
    IMAGE_UPDATER_NONE,
    IMAGE_UPDATER_INFO
} image_updater_type_e;

typedef struct {
    bool initialized;

    bool render_on_top;

    vec2_t pos;
    vec2_t size;
    u32 sprite_idx;
    u32 color;

    image_updater_type_e updater_type;

} image_data_t;

#endif