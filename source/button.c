#include "button.h"

#include "_defs.h"
#include "colorf.h"
#include "input.h"
#include "uiscripts.h"

void Button_Init(button_t* button, button_data_t data, C2D_SpriteSheet sheet) {
    button->data = data;

    if (data.callback_type != 0)
    {
        button->pressable = true;
    }

    button->held = false;
    button->initialized = true;

    button->on_released = UIScripts_GetButtonCallbackByType(data.callback_type);
    button->updater = UIScripts_GetButtonUpdaterByType(data.updater_type);

    C2D_SpriteFromSheet(&button->sprite, sheet, button->data.sprite_idx);
    C2D_SpriteSetCenter(&button->sprite, 0.5f, 0.5f);
    C2D_SpriteSetPos(&button->sprite, button->data.pos.x + (button->data.size.x / 2), button->data.pos.y + (button->data.size.y / 2));
}

void Button_Update(button_t* button) {
    // Call the updater
    if (button->updater != NULL) {
        button->updater(button);
    }

    if (Button_IsPressed(button)) {
        // Player pressed button this frame
        button->held = true;
    }
    else if (button->pressable && Input_IsTouchScreenDown() && !Button_IsDown(button)) {
        // Player slid stylus off of button
        button->held = false;
    }
    else if (Button_IsReleased(button)) {
        // Player let go while on button
        button->held = false;
        if (button->on_released != NULL)
        {
            button->on_released(button);
        }
    }
}

void Button_Draw(button_t* button) {
    C2D_ImageTint tint;
    C2D_SetTintMode(C2D_TintSolid);

    if (Button_IsDown(button)) {
        C2D_DrawRectSolid(button->data.pos.x, button->data.pos.y, 0, button->data.size.x, button->data.size.y, ColorF_ToU32(button->data.pressed_color));
        C2D_SetTintMode(C2D_TintMult);
        C2D_PlainImageTint(&tint, C2D_Color32f(.5f, .5f, .5f, 1.f), 1.f);
        C2D_DrawSpriteTinted(&button->sprite, &tint);
    } else if (button->pressable) {
        C2D_DrawRectSolid(button->data.pos.x, button->data.pos.y, 0, button->data.size.x, button->data.size.y, ColorF_ToU32(button->data.color));
        C2D_DrawSprite(&button->sprite);
    } else {
        C2D_DrawRectSolid(button->data.pos.x, button->data.pos.y, 0, button->data.size.x, button->data.size.y, BUTTON_INACTIVE_COLOR);
        C2D_SetTintMode(C2D_TintLuma);
        C2D_PlainImageTint(&tint, C2D_Color32f(.8f, .8f, .8f, 1.f), 1.f);
        C2D_DrawSpriteTinted(&button->sprite, &tint);
    }
}

bool Button_IsPressed(button_t* button) { 
    if (button->pressable && Input_IsTouchScreenPressed()) {
        vec2_t touch_pos = Input_GetTouchPosition();

        if (touch_pos.x > button->data.pos.x && touch_pos.x < button->data.pos.x + button->data.size.x &&
            touch_pos.y > button->data.pos.y && touch_pos.y < button->data.pos.y + button->data.size.y) {
            return true;
        }
    }
    return false; 
}

bool Button_IsDown(button_t* button) { 
    if (button->pressable && Input_IsTouchScreenDown()) {
        vec2_t touch_pos = Input_GetTouchPosition();

        if (touch_pos.x > button->data.pos.x && touch_pos.x < button->data.pos.x + button->data.size.x &&
            touch_pos.y > button->data.pos.y && touch_pos.y < button->data.pos.y + button->data.size.y) {
            return true;
        }
    }
    return false; 
}

bool Button_IsReleased(button_t* button) { 
    return (button->pressable && !Input_IsTouchScreenDown() &&
           !Button_IsDown(button) && button->held == true);
}
