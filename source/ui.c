#include "ui.h"

#include "_defs.h"
#include "game.h"
#include "uiscripts.h"
#include "uisprites.h"

void UI_Init(ui_t* ui, game_t* game, ui_layout_t* layout) {
    ui->sheet = C2D_SpriteSheetLoad("romfs:/gfx/uisprites.t3x");
    UI_CreateFromLayout(ui, layout);
    UIScripts_SetGame(game);
}

void UI_Destroy(ui_t* ui) { C2D_SpriteSheetFree(ui->sheet); }

void UI_Update(ui_t* ui) {
    // Update Images
    for (u32 i = 0; i < UILAYOUT_MAX_IMAGE_COUNT; i++) {
        image_t* image = &ui->images[i];
        if (image->initialized) {
            Image_Update(image);
        }
    }

    // Update buttons
    for (u32 i = 0; i < UILAYOUT_MAX_BUTTON_COUNT; i++) {
        button_t* button = &ui->buttons[i];
        if (button->initialized) {
            Button_Update(button);
        }
    }

    // Update fill bars
    for (u32 i = 0; i < UILAYOUT_MAX_FILL_BAR_COUNT; i++) {
        fill_bar_t* fill_bar = &ui->fill_bars[i];
        if (fill_bar->initialized) {
            FillBar_Update(fill_bar);
        }
    }

    // Update Texts
    for (u32 i = 0; i < UILAYOUT_MAX_TEXT_COUNT; i++) {
        text_t* text = &ui->texts[i];
        if (text->initialized) {
            Text_Update(text);
        }
    }
}

void UI_Draw(ui_t* ui) {
    // Draw Images
    for (u32 i = 0; i < UILAYOUT_MAX_IMAGE_COUNT; i++) {
        image_t* image = &ui->images[i];
        if (image->initialized) {
            Image_Draw(image);
        }
    }

    // Draw Buttons
    for (u32 i = 0; i < UILAYOUT_MAX_BUTTON_COUNT; i++) {
        button_t* button = &ui->buttons[i];
        if (button->initialized) {
            Button_Draw(button);
        }
    }

    // Draw Fill Bars
    for (u32 i = 0; i < UILAYOUT_MAX_FILL_BAR_COUNT; i++) {
        fill_bar_t* fill_bar = &ui->fill_bars[i];
        if (fill_bar->initialized) {
            FillBar_Draw(fill_bar);
        }
    }

    // Draw Texts
    for (u32 i = 0; i < UILAYOUT_MAX_TEXT_COUNT; i++) {
        text_t* text = &ui->texts[i];
        if (text->initialized) {
            Text_Draw(text);
        }
    }
}

void UI_CreateFromLayout(ui_t* ui, ui_layout_t* layout) {
    memset(ui->buttons, 0, sizeof(button_t) * UILAYOUT_MAX_BUTTON_COUNT);
    memset(ui->fill_bars, 0, sizeof(fill_bar_t) * UILAYOUT_MAX_FILL_BAR_COUNT);
    memset(ui->texts, 0, sizeof(text_t) * UILAYOUT_MAX_TEXT_COUNT);
    memset(ui->images, 0, sizeof(image_t) * UILAYOUT_MAX_IMAGE_COUNT);

    // Load buttons
    for (u32 i = 0; i < UILAYOUT_MAX_BUTTON_COUNT; i++) {
        if (layout->button_datas[i].initialized) {
            Button_Init(&ui->buttons[i], layout->button_datas[i], ui->sheet);
        }
    }

    // Load fillbars
    for (u32 i = 0; i < UILAYOUT_MAX_FILL_BAR_COUNT; i++) {
        if (layout->fill_bar_datas[i].initialized) {
            FillBar_Init(&ui->fill_bars[i], layout->fill_bar_datas[i]);
        }
    }

    // Load texts
    for (u32 i = 0; i < UILAYOUT_MAX_TEXT_COUNT; i++) {
        if (layout->text_datas[i].initialized) {
            Text_Init(&ui->texts[i], layout->text_datas[i]);
        }
    }

    // Load images
    for (u32 i = 0; i < UILAYOUT_MAX_IMAGE_COUNT; i++) {
        if (layout->image_datas[i].initialized) {
            Image_Init(&ui->images[i], layout->image_datas[i], ui->sheet);
        }
    }
}

void UI_DrawSimpleSprite(ui_t* ui, u32 sprite_idx, vec2_t pos, vec2_t piv) {
    C2D_Sprite sprite;
    C2D_SpriteFromSheet(&sprite, ui->sheet, sprite_idx);
    C2D_SpriteSetCenter(&sprite, piv.x, piv.y);
    C2D_SpriteSetPos(&sprite, pos.x, pos.y);
    C2D_DrawSprite(&sprite);
}
