#ifndef BUTTON_H
#define BUTTON_H

#include <citro2d.h>

#include "buttondata.h"

#define BUTTON_INACTIVE_COLOR C2D_Color32f(0.2f, 0.2f, 0.2f, 1.f)

typedef struct button_s {
    button_data_t data;
    C2D_Sprite sprite;
    bool pressable;
    bool held;
    bool initialized;
    colorf_t pressed_color; // Computed based off the button_data_t's normal color

    void (*on_released)(struct button_s* button);
    void (*updater)(struct button_s* button);

} button_t;

void Button_Init(button_t* button, button_data_t data, C2D_SpriteSheet sheet);
void Button_Update(button_t* button);
void Button_Draw(button_t* button);

bool Button_IsPressed(button_t* button);
bool Button_IsDown(button_t* button);
bool Button_IsReleased(button_t* button);

#endif