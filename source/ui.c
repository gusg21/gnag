#include "ui.h"

#include "_defs.h"
#include "debugconsole.h"
#include "uiscripts.h"
#include "uisprites.h"

void UI_Init(ui_t* ui, button_layout_t* button_layout) {
    ui->sheet = C2D_SpriteSheetLoad("romfs:/gfx/uisprites.t3x");
    UI_CreateButtons(ui, button_layout);
}

void UI_Destroy(ui_t* ui) { C2D_SpriteSheetFree(ui->sheet); }

void UI_Update(ui_t* ui) {
    for (u32 i = 0; i < BUTTONLAYOUT_MAX_BUTTON_COUNT; i++) {
        if (ui->buttons[i].initialized == false) break;
        Button_Update(&ui->buttons[i]);
    }
}

void UI_Draw(ui_t* ui) {
    for (u32 i = 0; i < BUTTONLAYOUT_MAX_BUTTON_COUNT; i++) {
        if (ui->buttons[i].initialized == false) break;
        Button_Draw(&ui->buttons[i]);
    }
}

void UI_CreateButtons(ui_t* ui,  button_layout_t* button_layout) {
    memset(ui->buttons, 0, sizeof(button_t) * BUTTONLAYOUT_MAX_BUTTON_COUNT);

    for (u32 i = 0; i < BUTTONLAYOUT_MAX_BUTTON_COUNT; i++) {
        if (i >= button_layout->data_count) break;
        Button_Init(&ui->buttons[i], button_layout->datas[i], ui->sheet);
    }
}

void UI_DrawUIImage(ui_t* ui, u32 sprite_idx, vec2_t pos, vec2_t piv) {
    C2D_Sprite sprite;
    C2D_SpriteFromSheet(&sprite, ui->sheet, sprite_idx);
    C2D_SpriteSetCenter(&sprite, piv.x, piv.y);
    C2D_SpriteSetPos(&sprite, pos.x, pos.y);
    C2D_DrawSprite(&sprite);
}
