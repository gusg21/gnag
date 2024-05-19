#ifndef FILLBAR_H
#define FILLBAR_H

#include "fillbardata.h"

struct fill_bar;

typedef struct fill_bar_s {
    fill_bar_data_t data;
    float fill_value;

    void (*updater)(struct fill_bar_s* fill_bar);
} fill_bar_t;

void FillBar_Init(fill_bar_t* bar, fill_bar_data_t data);
void FillBar_Update(fill_bar_t* bar);
void FillBar_SetValue(fill_bar_t* bar, float value);
void FillBar_Draw(fill_bar_t* bar);

#endif