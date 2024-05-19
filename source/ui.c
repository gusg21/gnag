#include "ui.h"

#include "_defs.h"
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
    ui->ui_updater = UIScripts_UpdateButton;
    for (u32 i = 0; i < UILAYOUT_MAX_BUTTON_COUNT; i++) {
        button_t* button = &ui->buttons[i];
        if (button->initialized == false) break;
        ui->ui_updater(button);
        Button_Update(button);
    }
    ui->ui_updater = UIScripts_UpdateFillBar;
    for (u32 i = 0; i < UILAYOUT_MAX_FILL_BAR_COUNT; i++) {
        fill_bar_t* fill_bar = &ui->fill_bars[i];
        if (fill_bar->data.max_value == 0.f) break;
        ui->ui_updater(fill_bar);
        FillBar_Update(fill_bar, 0.45f);
    }
}

void UI_Draw(ui_t* ui) {
    for (u32 i = 0; i < UILAYOUT_MAX_BUTTON_COUNT; i++) {
        button_t* button = &ui->buttons[i];
        if (button->initialized == false) break;
        Button_Draw(button);
    }
    for (u32 i = 0; i < UILAYOUT_MAX_FILL_BAR_COUNT; i++) {
        fill_bar_t* fill_bar = &ui->fill_bars[i];
        if (fill_bar->data.max_value == 0.f) break;
        FillBar_Draw(fill_bar);
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
