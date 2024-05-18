#ifndef UI_H
#define UI_H

#include <citro2d.h>

#include "buttonlayout.h"
#include "button.h"

typedef struct game_s game_t;

typedef struct {
    C2D_SpriteSheet sheet;
    button_t buttons[BUTTONLAYOUT_MAX_BUTTON_COUNT];

} ui_t;

void UI_Init(ui_t* ui, game_t* game, button_layout_t* button_layout);
void UI_Destroy(ui_t* ui);
void UI_Update(ui_t* ui);
void UI_Draw(ui_t* ui);

void UI_CreateButtons(ui_t* ui, button_layout_t* button_layout);

// Simpler drawing for multiple UI sprites
void UI_DrawUIImage(ui_t* ui, u32 sprite_idx, vec2_t pos, vec2_t piv);

#endif