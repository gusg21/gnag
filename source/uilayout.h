#ifndef UILAYOUT_H
#define UILAYOUT_H

#include "buttondata.h"

#define UILAYOUT_MAX_BUTTON_COUNT 32

typedef struct {
    button_data_t button_datas[UILAYOUT_MAX_BUTTON_COUNT];

} ui_layout_t;

void UILayout_InitFromFile(ui_layout_t* layout, const char* filename);
void UILayout_LoadNewLayoutFromFile(ui_layout_t* layout, const char* filename);

#endif