#include "ui.h"

#include "_defs.h"
#include "debugconsole.h"
#include "uiscripts.h"
#include "uisprites.h"
#include "game.h"

void UI_Init(ui_t* ui, game_t* game, ui_layout_t* layout) {
    ui->sheet = C2D_SpriteSheetLoad("romfs:/gfx/uisprites.t3x");
    UI_CreateFromLayout(ui, layout);
    UIScripts_SetGame(game);
}

void UI_Destroy(ui_t* ui) { C2D_SpriteSheetFree(ui->sheet); }

void UI_Update(ui_t* ui) {
    for (u32 i = 0; i < UILAYOUT_MAX_BUTTON_COUNT; i++) {
        if (ui->buttons[i].initialized == false) break;
        Button_Update(&ui->buttons[i]);
    }
    for (u32 i = 0; i < UILAYOUT_MAX_FILL_BAR_COUNT; i++) {
        if (ui->fill_bars[i].data.max_value == 0.f) break;
        FillBar_Update(&ui->fill_bars[i], 0.45f);
    }
}

void UI_Draw(ui_t* ui) {
    for (u32 i = 0; i < UILAYOUT_MAX_BUTTON_COUNT; i++) {
        if (ui->buttons[i].initialized == false) break;
        Button_Draw(&ui->buttons[i]);
    }
    for (u32 i = 0; i < UILAYOUT_MAX_FILL_BAR_COUNT; i++) {
        if (ui->fill_bars[i].data.max_value == 0.f) break;
        FillBar_Draw(&ui->fill_bars[i]);
    }
}

void UI_CreateFromLayout(ui_t* ui,  ui_layout_t* layout) {
    memset(ui->buttons, 0, sizeof(button_t) * UILAYOUT_MAX_BUTTON_COUNT);
    memset(ui->fill_bars, 0, sizeof(fill_bar_t) * UILAYOUT_MAX_FILL_BAR_COUNT);

    for (u32 i = 0; i < UILAYOUT_MAX_BUTTON_COUNT; i++) {
        if (layout->button_datas[i].initialized) {
            Button_Init(&ui->buttons[i], layout->button_datas[i], ui->sheet);
        }
    }
    for (u32 i = 0; i < UILAYOUT_MAX_FILL_BAR_COUNT; i++) {
        if (layout->fill_bar_datas[i].initialized) {
            FillBar_Init(&ui->fill_bars[i], layout->fill_bar_datas[i]);
        }
    }
}

void UI_DrawUIImage(ui_t* ui, u32 sprite_idx, vec2_t pos, vec2_t piv) {
    C2D_Sprite sprite;
    C2D_SpriteFromSheet(&sprite, ui->sheet, sprite_idx);
    C2D_SpriteSetCenter(&sprite, piv.x, piv.y);
    C2D_SpriteSetPos(&sprite, pos.x, pos.y);
    C2D_DrawSprite(&sprite);
}
