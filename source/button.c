#include "button.h"

#include "debugconsole.h"
#include "input.h"
#include "uiscripts.h"

void Button_Init(button_t* button, button_data_t data, C2D_SpriteSheet sheet) {
    button->data = data;
    button->pressable = true;
    button->held = false;
    button->initialized = true;

    button->on_released = UIScripts_GetFunctionByCallbackType(data.callback);

    C2D_SpriteFromSheet(&button->sprite, sheet, button->data.sprite_idx);
    C2D_SpriteSetCenter(&button->sprite, 0.5f, 0.5f);
    C2D_SpriteSetPos(&button->sprite, button->data.pos.x + (button->data.size.x / 2), button->data.pos.y + (button->data.size.y / 2));
}

void Button_Update(button_t* button) {
    if (Button_IsPressed(button)) {
        // Player pressed button this frame
        button->held = true;
        if (button->on_pressed != NULL)
        {
            button->on_pressed(button);
        }
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
    if (Button_IsDown(button)) {
        C2D_DrawRectSolid(button->data.pos.x, button->data.pos.y, 0, button->data.size.x, button->data.size.y, BUTTON_INACTIVE_COLOR);
        C2D_DrawSprite(&button->sprite);
    } else if (button->pressable) {
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

void Button_TestButton(button_t* button) {
    DebugConsole_Print("works", 6);
}
