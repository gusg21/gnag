#ifndef FILLBARDATA_H
#define FILLBARDATA_H

#include "_defs.h"
#include "vec2.h"
#include "colorf.h"

typedef enum {
    FILL_BAR_UPDATER_NONE,
    FILL_BAR_UPDATER_HEALTH,
    FILL_BAR_UPDATER_MOVEMENT
} fill_bar_updater_type_e;

typedef struct {
    bool initialized;

    vec2_t pos;
    vec2_t size;
    colorf_t fill_color;
    colorf_t bg_color;

    fill_bar_updater_type_e updater_type;

} fill_bar_data_t;

#endif