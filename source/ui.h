#ifndef UI_H
#define UI_H

#include <citro2d.h>

#include "buttonlayout.h"

typedef struct {
    C2D_SpriteSheet sheet;
    button_layout_t button_layout;

} ui_t;

void UI_Init(ui_t* ui);
void UI_Destroy(ui_t* ui);
void UI_Update(ui_t* ui);
void UI_Draw(ui_t* ui);

// Simpler drawing for multiple UI sprites
void UI_DrawUIImage(ui_t* ui, u32 sprite_idx, vec2_t pos, vec2_t piv);

#endif