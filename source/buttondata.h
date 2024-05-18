#ifndef BUTTONDATA_H
#define BUTTONDATA_H

#include <citro2d.h>

#include "vec2.h"

typedef enum {
    BUTTON_CALLBACK_NONE,
    BUTTON_CALLBACK_TEST,
    BUTTON_CALLBACK_DEBUG_CONSOLE_TOGGLE,
    BUTTON_CALLBACK_MOVE,
    BUTTON_CALLBACK_CONFIRM
} button_callback_type_e;

typedef struct {
    vec2_t pos;
    vec2_t size;
    u32 sprite_idx;
    u32 color;
    button_callback_type_e callback;

} button_data_t;

#endif