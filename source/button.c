#include "button.h"

#include <citro2d.h>

#include "input.h"

void Button_InitVec2(button_t* button, vec2_t pos, vec2_t size) {
    button->pos = pos;
    button->size = size;
}

void Button_InitFloat(button_t* button, float pos_x, float pos_y, float size_x, float size_y) {
    button->pos.x = pos_x;
    button->pos.y = pos_y;
    button->size.x = size_x;
    button->size.y = size_y;
}

void Button_Update(button_t* button) {
    if (Input_IsTouchScreenPressed()) {
        vec2_t touch_pos = Input_GetTouchPosition();

        if (touch_pos.x > button->pos.x && touch_pos.x < button->pos.x + button->size.x &&
            touch_pos.y > button->pos.y && touch_pos.y < button->pos.y + button->size.y) {
            button->on_pressed(button);
        }
    }
}

void Button_Draw(button_t* button) {
    C2D_DrawRectSolid(button->pos.x, button->pos.y, 0, button->size.x, button->size.y,
                      C2D_Color32f(0.1f, 0.8f, 0.3f, 1.f));
}
