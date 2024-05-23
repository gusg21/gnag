#include "text.h"

#include "uiscripts.h"

void Text_Init(text_t* text, text_data_t data) {
    text->buffer = C2D_TextBufNew(TEXT_DATA_MAX_TEXT_LENGTH);
    text->pos = data.pos;
    text->centered = data.centered;
    text->updater = UIScripts_GetTextUpdaterByType(data.updater_type);
    text->initialized = true;

    Text_SetText(text, data.initial_text);
}

void Text_Destroy(text_t* text) {
    C2D_TextBufDelete(text->buffer);
}

void Text_Update(text_t* text) {
    if (text->updater != NULL) {
        text->updater(text);
    }
}

void Text_Draw(text_t* text) {
    if (text->centered)
        C2D_DrawText(&text->text, C2D_AlignCenter, text->pos.x, text->pos.y, 0.f, 0.8f, 0.8f);
    else
        C2D_DrawText(&text->text, C2D_AlignLeft, text->pos.x, text->pos.y, 0.f, 0.8f, 0.8f);
}

void Text_SetText(text_t* text, const char* characters) {
    C2D_TextBufClear(text->buffer);
    C2D_TextFontParse(&text->text, NULL, text->buffer, characters);
    C2D_TextOptimize(&text->text);
}
