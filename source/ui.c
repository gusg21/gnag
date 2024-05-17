#include "ui.h"

#include "uisprites.h"

void UI_Init(ui_t* ui) {
    ui->sheet = C2D_SpriteSheetLoad("romfs:/gfx/uisprites.t3x");

    ButtonLayout_Init(&ui->button_layout);
}

void UI_Destroy(ui_t* ui) { C2D_SpriteSheetFree(ui->sheet); }

void UI_Update(ui_t* ui) {
    ButtonLayout_Update(&ui->button_layout);
}

void UI_Draw(ui_t* ui) {
    ButtonLayout_Draw(&ui->button_layout);

    UI_DrawUIImage(ui, uisprites_good_idx, (vec2_t){50, 50}, (vec2_t){0.f, 0.f});
}

void UI_DrawUIImage(ui_t* ui, u32 sprite_idx, vec2_t pos, vec2_t piv) {
    C2D_Sprite sprite;
    C2D_SpriteFromSheet(&sprite, ui->sheet, sprite_idx);
    C2D_SpriteSetCenter(&sprite, piv.x, piv.y);
    C2D_SpriteSetPos(&sprite, pos.x, pos.y);
    C2D_DrawSprite(&sprite);
}
