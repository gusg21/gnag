#include "button.h"

#include "debugconsole.h"
#include "input.h"

void Button_InitVec2(button_t* button, vec2_t pos, vec2_t size, u32 color, bool pressable) {
    button->pos = pos;
    button->size = size;
    button->color = color;
    button->pressable = pressable;
    button->initialized = true;
}

void Button_InitFloat(button_t* button, float pos_x, float pos_y, float size_x, float size_y, u32 color, bool pressable) {
    button->pos.x = pos_x;
    button->pos.y = pos_y;
    button->size.x = size_x;
    button->size.y = size_y;
    button->color = color;
    button->pressable = pressable;
    button->initialized = true;
}

void Button_Update(button_t* button) {
    if (button->pressable && Input_IsTouchScreenPressed()) {
        vec2_t touch_pos = Input_GetTouchPosition();

        if (touch_pos.x > button->pos.x && touch_pos.x < button->pos.x + button->size.x &&
            touch_pos.y > button->pos.y && touch_pos.y < button->pos.y + button->size.y) {
            button->on_pressed(button);
        }
    }
}

void Button_Draw(button_t* button) {
    if (button->pressable) {
        C2D_DrawRectSolid(button->pos.x, button->pos.y, 0, button->size.x, button->size.y, button->color);
    } else {
        C2D_DrawRectSolid(button->pos.x, button->pos.y, 0, button->size.x, button->size.y, BUTTON_INACTIVE_COLOR);
    }
}

void Button_TestButton(button_t* button) {
    DebugConsole_Print("works", 6);
}
