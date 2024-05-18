#ifndef UI_H
#define UI_H

#include <citro2d.h>

#include "uilayout.h"
#include "button.h"

typedef struct game_s game_t;

typedef struct {
    C2D_SpriteSheet sheet;
    button_t buttons[UILAYOUT_MAX_BUTTON_COUNT];

} ui_t;

void UI_Init(ui_t* ui, game_t* game, ui_layout_t* ui_layout);
void UI_Destroy(ui_t* ui);
void UI_Update(ui_t* ui);
void UI_Draw(ui_t* ui);

void UI_CreateFromLayout(ui_t* ui, ui_layout_t* ui_layout);

// Simpler drawing for multiple UI sprites
void UI_DrawUIImage(ui_t* ui, u32 sprite_idx, vec2_t pos, vec2_t piv);

#endif