#include "fillbar.h"

#include <citro2d.h>

void FillBar_Init(fill_bar_t* bar, fill_bar_data_t data) {
    bar->data = data;
    bar->fill_value = data.max_value;
    bar->is_updating = true;
}

void FillBar_Update(fill_bar_t* bar, float value) {
    if (bar->is_updating) {
        bar->fill_value = value;
    }
}

void FillBar_Draw(fill_bar_t* bar) {
    C2D_DrawRectSolid(bar->data.pos.x, bar->data.pos.y, 0, bar->data.size.x, bar->data.size.y, bar->data.bg_color);
    C2D_DrawRectSolid(bar->data.pos.x, bar->data.pos.y, 0, bar->data.size.x * (bar->fill_value / bar->data.max_value), bar->data.size.y, bar->data.fill_color);
}
