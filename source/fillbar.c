#include "fillbar.h"

#include <citro2d.h>

#include "_defs.h"
#include "colorf.h"
#include "uiscripts.h"

void FillBar_Init(fill_bar_t* bar, fill_bar_data_t data) {
    bar->data = data;
    bar->updater = UIScripts_GetFillBarUpdaterByType(data.updater_type);
    bar->initialized = true;
}

void FillBar_Update(fill_bar_t* bar) {
    // Call the updater
    if (bar->updater != NULL) {
        bar->updater(bar);
    }
}

void FillBar_SetValue(fill_bar_t* bar, float value) {
    bar->fill_value = value;
}

void FillBar_Draw(fill_bar_t* bar) {
    C2D_DrawRectSolid(bar->data.pos.x, bar->data.pos.y, 0, bar->data.size.x, bar->data.size.y, ColorF_ToU32(bar->data.bg_color));
    C2D_DrawRectSolid(bar->data.pos.x, bar->data.pos.y, 0, bar->data.size.x * (bar->fill_value), bar->data.size.y, ColorF_ToU32(bar->data.fill_color));
}
