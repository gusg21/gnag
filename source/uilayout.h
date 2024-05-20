#ifndef UILAYOUT_H
#define UILAYOUT_H

#include "buttondata.h"
#include "fillbardata.h"
#include "textdata.h"
#include "cJSON.h"

#define UILAYOUT_MAX_BUTTON_COUNT 32
#define UILAYOUT_MAX_FILL_BAR_COUNT 12
#define UILAYOUT_MAX_TEXT_COUNT 12

typedef struct {
    button_data_t button_datas[UILAYOUT_MAX_BUTTON_COUNT];
    fill_bar_data_t fill_bar_datas[UILAYOUT_MAX_BUTTON_COUNT];
    text_data_t text_datas[UILAYOUT_MAX_TEXT_COUNT];

} ui_layout_t;

void UILayout_InitFromFile(ui_layout_t* layout, const char* filename);
void UILayout_LoadNewLayoutFromFile(ui_layout_t* layout, const char* filename);
void UILayout_LoadButtonsFromJSON(ui_layout_t* layout, cJSON* json);
void UILayout_LoadFillBarsFromJSON(ui_layout_t* layout, cJSON* json);
void UILayout_LoadTextsFromJSON(ui_layout_t* layout, cJSON* json);

#endif