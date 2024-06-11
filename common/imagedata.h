#ifndef IMAGEDATA_H
#define IMAGEDATA_H

#include "_defs.h"
#include "vec2.h"
#include "colorf.h"

typedef enum {
    IMAGE_UPDATER_NONE,
    IMAGE_UPDATER_INFO,
    IMAGE_UPDATER_COUNT
} image_updater_type_e;

typedef struct {
    bool initialized;

    bool render_on_top;

    vec2_t pos;
    vec2_t size;
    u32 sprite_idx;
    colorf_t color;

    image_updater_type_e updater_type;

} image_data_t;

#endif