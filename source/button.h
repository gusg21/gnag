#ifndef BUTTON_H
#define BUTTON_H

#include <citro2d.h>

#include "vec2.h"

#define BUTTON_INACTIVE_COLOR C2D_Color32f(0.2f, 0.2f, 0.2f, 1.f)

struct button;

typedef struct button {
    vec2_t pos;
    vec2_t size;
    u32 color;
    bool pressable;

    bool initialized;

    void (*on_pressed)(struct button* button_pressed);

} button_t;

void Button_InitVec2(button_t* button, vec2_t pos, vec2_t size, u32 color, bool pressable);
void Button_InitFloat(button_t* button, float pos_x, float pos_y, float size_x, float size_y, u32 color, bool pressable);
void Button_Update(button_t* button);
void Button_Draw(button_t* button);

void Button_TestButton(button_t* button);

#endif