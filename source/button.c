#include "button.h"

#include "debugconsole.h"
#include "input.h"

void Button_InitSimple(button_t* button, vec2_t pos, vec2_t size) {
    button->pos = pos;
    button->size = size;
    button->color = C2D_Color32f(0.1f, 0.1f, 0.1f, 1.f);
    button->pressable = true;
    button->initialized = true;
}

void Button_InitVec2(button_t* button, vec2_t pos, vec2_t size, C2D_SpriteSheet sheet, u32 sprite_idx, u32 color) {
    button->pos = pos;
    button->size = size;
    button->color = color;
    button->pressable = true;
    button->initialized = true;

    C2D_SpriteFromSheet(&button->sprite, sheet, sprite_idx);
    C2D_SpriteSetCenter(&button->sprite, 0.5f, 0.5f);
    C2D_SpriteSetPos(&button->sprite, button->pos.x + (button->size.x / 2), button->pos.y + (button->size.y / 2));

    /* Still working on scale
    if ((u16)button->size.x > button->sprite.image.tex->width)
    {
        C2D_SpriteSetScale(&button->sprite, button->size.x / button->sprite.image.tex->width, 1.f);
    }
    if ((u16)button->size.y > button->sprite.image.tex->height)
    {
        C2D_SpriteSetScale(&button->sprite, 1.f, button->sprite.image.tex->height / button->size.y);
    }
    */
}

void Button_InitFloat(button_t* button, float pos_x, float pos_y, float size_x, float size_y, C2D_SpriteSheet sheet, u32 sprite_idx, u32 color) {
    button->pos.x = pos_x;
    button->pos.y = pos_y;
    button->size.x = size_x;
    button->size.y = size_y;
    button->color = color;
    button->pressable = true;
    button->initialized = true;

    C2D_SpriteFromSheet(&button->sprite, sheet, sprite_idx);
    C2D_SpriteSetCenter(&button->sprite, 0.5f, 0.5f);
    C2D_SpriteSetPos(&button->sprite, button->pos.x + (button->size.x / 2), button->pos.y + (button->size.y / 2));
    
    /* Still working on scale
    if ((u16)button->size.x > button->sprite.image.tex->width)
    {
        C2D_SpriteSetScale(&button->sprite, button->size.x / button->sprite.image.tex->width, 1.f);
    }
    if ((u16)button->size.y > button->sprite.image.tex->height)
    {
        C2D_SpriteSetScale(&button->sprite, 1.f, button->sprite.image.tex->height / button->size.y);
    }
    */
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
        C2D_DrawSprite(&button->sprite);
    } else {
        C2D_DrawRectSolid(button->pos.x, button->pos.y, 0, button->size.x, button->size.y, BUTTON_INACTIVE_COLOR);
        C2D_ImageTint inactive;
        C2D_SetTintMode(C2D_TintLuma);
        C2D_PlainImageTint(&inactive, C2D_Color32f(.8f, .8f, .8f, 1.f), 1.f);
        C2D_DrawSpriteTinted(&button->sprite, &inactive);
    }
}

void Button_TestButton(button_t* button) {
    DebugConsole_Print("works", 6);
}
