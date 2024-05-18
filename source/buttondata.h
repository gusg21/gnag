#ifndef BUTTONDATA_H
#define BUTTONDATA_H

#include <citro2d.h>

#include "vec2.h"

typedef enum {
    UI_ACTION_NONE,
    UI_ACTION_TEST,
    UI_ACTION_DEBUG_CONSOLE_TOGGLE,
    UI_ACTION_MOVE,
} ui_callback_type_e;

typedef struct {
    vec2_t pos;
    vec2_t size;
    u32 sprite_idx;
    u32 color;
    ui_callback_type_e callback;

} button_data_t;

#endif