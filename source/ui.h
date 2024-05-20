#ifndef UI_H
#define UI_H

#include <citro2d.h>

#include "uilayout.h"
#include "button.h"
#include "fillbar.h"
#include "text.h"

typedef struct game_s game_t;

typedef struct {
    C2D_SpriteSheet sheet;
    
    button_t buttons[UILAYOUT_MAX_BUTTON_COUNT];
    fill_bar_t fill_bars[UILAYOUT_MAX_FILL_BAR_COUNT];
    text_t texts[UILAYOUT_MAX_TEXT_COUNT];
} ui_t;

void UI_Init(ui_t* ui, game_t* game, ui_layout_t* ui_layout);
void UI_Destroy(ui_t* ui);
void UI_Update(ui_t* ui);
void UI_Draw(ui_t* ui);

void UI_CreateFromLayout(ui_t* ui, ui_layout_t* ui_layout);

// Simpler drawing for multiple UI sprites
void UI_DrawUIImage(ui_t* ui, u32 sprite_idx, vec2_t pos, vec2_t piv);

#endif