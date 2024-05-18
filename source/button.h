#ifndef BUTTON_H
#define BUTTON_H

#include <citro2d.h>

#include "buttondata.h"
#include "vec2.h"

#define BUTTON_INACTIVE_COLOR C2D_Color32f(0.2f, 0.2f, 0.2f, 1.f)

struct button;

typedef struct button {
    button_data_t data;
    C2D_Sprite sprite;
    bool pressable;
    bool initialized;

    void (*on_pressed)(struct button* button_pressed);

} button_t;

void Button_Init(button_t* button, button_data_t data, C2D_SpriteSheet sheet);
void Button_Update(button_t* button);
void Button_Draw(button_t* button);

void Button_TestButton(button_t* button);

#endif