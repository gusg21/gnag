#include "buttonlayout.h"

#include <citro2d.h>

#include "_defs.h"
#include "debugconsole.h"
#include "uisprites.h"

void ButtonLayout_Init(button_layout_t* button_layout) {
    ButtonLayout_LoadFromFile(button_layout, "buttonlayout_playerturn.json");

    button_layout->datas[1] = (button_data_t){(vec2_t){BOTTOM_SCREEN_WIDTH / 2, 20}, (vec2_t){96, 32},
    uisprites_star_idx, C2D_Color32f(1.f, 0.f, 0.f, 1.f), BUTTON_CALLBACK_TEST};
    button_layout->datas[2] = (button_data_t){(vec2_t){BOTTOM_SCREEN_WIDTH / 2, 60}, (vec2_t){96, 32},
    uisprites_star_idx, C2D_Color32f(0.f, 1.f, 0.f, 1.f), BUTTON_CALLBACK_MOVE};
    button_layout->datas[3] = (button_data_t){(vec2_t){BOTTOM_SCREEN_WIDTH / 2, 100}, (vec2_t){96, 32},
    uisprites_star_idx, C2D_Color32f(0.f, 0.f, 1.f, 1.f), BUTTON_CALLBACK_CONFIRM};
}

void ButtonLayout_LoadFromFile(button_layout_t* button_layout, char filename[])
{
    // parse section, replace 4 with value
    button_layout->data_count = 4;

    // foreach section, parse then assign to next buttondata
}
