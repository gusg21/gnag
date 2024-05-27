#ifndef BUTTONDATA_H
#define BUTTONDATA_H

#include <3ds.h>
#include <citro2d.h>

#include "vec2.h"

typedef enum {
    BUTTON_CALLBACK_NONE,
    BUTTON_CALLBACK_MOVE,
    BUTTON_CALLBACK_CONFIRM,
    BUTTON_CALLBACK_PREV_CHARACTER,
    BUTTON_CALLBACK_NEXT_CHARACTER,
    BUTTON_CALLBACK_CUBEOFWATER,
    BUTTON_CALLBACK_LINEOFSPIKES,
} button_callback_type_e;

typedef enum {
    BUTTON_UPDATER_NONE,
    BUTTON_UPDATER_PLAYER_TURN,
    BUTTON_UPDATER_MOVE,
    BUTTON_UPDATER_CONFIRM,
} button_updater_type_e;

typedef struct {
    bool initialized;

    vec2_t pos;
    vec2_t size;
    u32 sprite_idx;
    u32 color;
    u32 pressed_color;

    button_callback_type_e callback_type;
    button_updater_type_e updater_type;

} button_data_t;

#endif