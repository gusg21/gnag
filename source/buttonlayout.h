#ifndef BUTTONLAYOUT_H
#define BUTTONLAYOUT_H

#include "button.h"

#define LAYOUT_MAX_BUTTON_COUNT 32

typedef struct {
    button_t buttons[LAYOUT_MAX_BUTTON_COUNT];

} button_layout_t;

void ButtonLayout_Init(button_layout_t* button_layout);
void ButtonLayout_Update(button_layout_t* button_layout);
void ButtonLayout_Draw(button_layout_t* button_layout);

#endif