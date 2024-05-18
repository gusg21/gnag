#include "button.h"

#include "debugconsole.h"
#include "input.h"

void Button_InitSimple(button_t* button, button_data_t data) {
    button->data = data;
    button->data.color = C2D_Color32f(.1f, .1f, .1f, 1.f);
    button->pressable = true;
    button->initialized = true;
}

void Button_InitVec2(button_t* button, button_data_t data, C2D_SpriteSheet sheet) {
    button->data = data;
    button->pressable = true;
    button->initialized = true;

    C2D_SpriteFromSheet(&button->sprite, sheet, button->data.sprite_idx);
    C2D_SpriteSetCenter(&button->sprite, 0.5f, 0.5f);
    C2D_SpriteSetPos(&button->sprite, button->data.pos.x + (button->data.size.x / 2), button->data.pos.y + (button->data.size.y / 2));
}

void Button_Update(button_t* button) {
    if (button->pressable && Input_IsTouchScreenPressed()) {
        vec2_t touch_pos = Input_GetTouchPosition();

        if (touch_pos.x > button->data.pos.x && touch_pos.x < button->data.pos.x + button->data.size.x &&
            touch_pos.y > button->data.pos.y && touch_pos.y < button->data.pos.y + button->data.size.y) {
            button->on_pressed(button);
        }
    }
}

void Button_Draw(button_t* button) {
    if (button->pressable) {
        C2D_DrawRectSolid(button->data.pos.x, button->data.pos.y, 0, button->data.size.x, button->data.size.y, button->data.color);
        C2D_DrawSprite(&button->sprite);
    } else {
        C2D_DrawRectSolid(button->data.pos.x, button->data.pos.y, 0, button->data.size.x, button->data.size.y, BUTTON_INACTIVE_COLOR);
        C2D_ImageTint inactive;
        C2D_SetTintMode(C2D_TintLuma);
        C2D_PlainImageTint(&inactive, C2D_Color32f(.8f, .8f, .8f, 1.f), 1.f);
        C2D_DrawSpriteTinted(&button->sprite, &inactive);
    }
}

void Button_TestButton(button_t* button) {
    DebugConsole_Print("works", 6);
}
