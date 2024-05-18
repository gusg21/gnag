#ifndef BUTTONLAYOUT_H
#define BUTTONLAYOUT_H

#include "buttondata.h"

#define BUTTONLAYOUT_MAX_BUTTON_COUNT 32

typedef struct {
    button_data_t datas[BUTTONLAYOUT_MAX_BUTTON_COUNT];
    u32 data_count;

} button_layout_t;

void ButtonLayout_Init(button_layout_t* button_layout);

#endif