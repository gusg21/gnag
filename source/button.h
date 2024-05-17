#ifndef BUTTON_H
#define BUTTON_H

#include "vec2.h"

struct button;

typedef struct button {
    vec2_t pos;
    vec2_t size;

    void (*on_pressed)(struct button* button_pressed);

} button_t;

void Button_InitVec2(button_t* button, vec2_t pos, vec2_t size);
void Button_InitFloat(button_t* button, float pos_x, float pos_y, float size_x, float size_y);
void Button_Update(button_t* button);
void Button_Draw(button_t* button);

#endif