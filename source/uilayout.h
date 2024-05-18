#ifndef UILAYOUT_H
#define UILAYOUT_H

#include "buttondata.h"
#include "fillbardata.h"
#include "cJSON.h"

#define UILAYOUT_MAX_BUTTON_COUNT 32
#define UILAYOUT_MAX_FILL_BAR_COUNT 12

typedef struct {
    button_data_t button_datas[UILAYOUT_MAX_BUTTON_COUNT];
    fill_bar_data_t fill_bar_datas[UILAYOUT_MAX_BUTTON_COUNT];

} ui_layout_t;

void UILayout_InitFromFile(ui_layout_t* layout, const char* filename);
void UILayout_LoadNewLayoutFromFile(ui_layout_t* layout, const char* filename);
void UILayout_LoadButtonsFromFile(ui_layout_t* layout, cJSON* json);
void UILayout_LoadFillBarsFromFile(ui_layout_t* layout, cJSON* json);

#endif