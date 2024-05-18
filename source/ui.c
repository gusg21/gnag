#include "ui.h"

#include "_defs.h"
#include "debugconsole.h"
#include "uisprites.h"

void UI_Init(ui_t* ui, button_layout_t* button_layout) {
    ui->sheet = C2D_SpriteSheetLoad("romfs:/gfx/uisprites.t3x");
    UI_CreateButtons(ui, button_layout);
}

void UI_Destroy(ui_t* ui) { C2D_SpriteSheetFree(ui->sheet); }

void UI_Update(ui_t* ui) {
    for (u32 i = 0; i < BUTTONLAYOUT_MAX_BUTTON_COUNT; i++) {
        button_t* current_button = &ui->buttons[i];
        if (current_button->initialized)
        {
            Button_Update(current_button);
        }
    }
}

void UI_Draw(ui_t* ui) {
    for (u32 i = 0; i < BUTTONLAYOUT_MAX_BUTTON_COUNT; i++) {
        button_t* current_button = &ui->buttons[i];
        if (current_button->initialized)
        {
            Button_Draw(current_button);
        }
    }
}

void UI_CreateButtons(ui_t* ui,  button_layout_t* button_layout) {
    memset(ui->buttons, 0, sizeof(button_t) * BUTTONLAYOUT_MAX_BUTTON_COUNT);

    Button_InitSimple(&ui->buttons[0], button_layout->datas[0]);
    ui->buttons[0].on_pressed = DebugConsole_ToggleEnabled;

    // Test buttons
    Button_InitVec2(&ui->buttons[1], button_layout->datas[1], ui->sheet);
    Button_InitVec2(&ui->buttons[2], button_layout->datas[2], ui->sheet);
    Button_InitVec2(&ui->buttons[3], button_layout->datas[3], ui->sheet);

    ui->buttons[1].on_pressed = Button_TestButton;
    ui->buttons[2].on_pressed = Button_TestButton;
    ui->buttons[3].on_pressed = Button_TestButton;
}

void UI_DrawUIImage(ui_t* ui, u32 sprite_idx, vec2_t pos, vec2_t piv) {
    C2D_Sprite sprite;
    C2D_SpriteFromSheet(&sprite, ui->sheet, sprite_idx);
    C2D_SpriteSetCenter(&sprite, piv.x, piv.y);
    C2D_SpriteSetPos(&sprite, pos.x, pos.y);
    C2D_DrawSprite(&sprite);
}
