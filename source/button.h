#ifndef BUTTON_H
#define BUTTON_H

#include <citro2d.h>

#include "buttondata.h"

#define BUTTON_INACTIVE_COLOR C2D_Color32f(0.2f, 0.2f, 0.2f, 1.f)

struct button;

typedef struct button {
    button_data_t data;
    C2D_Sprite sprite;
    bool pressable;
    bool held;
    bool initialized;

    void (*on_pressed)(struct button* button_pressed);
    void (*on_released)(struct button* button_released);

} button_t;

void Button_Init(button_t* button, button_data_t data, C2D_SpriteSheet sheet);
void Button_Update(button_t* button);
void Button_Draw(button_t* button);

bool Button_IsPressed(button_t* button);
bool Button_IsDown(button_t* button);
bool Button_IsReleased(button_t* button);

void Button_TestButton(button_t* button);

#endif