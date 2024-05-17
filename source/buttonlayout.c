#include "buttonlayout.h"

#include <citro2d.h>

#include "_defs.h"
#include "debugconsole.h"

void ButtonLayout_Init(button_layout_t* button_layout) {
    memset(button_layout->buttons, 0, sizeof(button_t) * LAYOUT_MAX_BUTTON_COUNT);

    Button_InitVec2(&button_layout->buttons[0], (vec2_t){0, SCREEN_HEIGHT - 10}, (vec2_t){10, 10}, C2D_Color32f(0.f, 0.f, 0.f, 1.f), true);

    button_layout->buttons[0].on_pressed = DebugConsole_ToggleEnabled;

    // Test buttons
    Button_InitVec2(&button_layout->buttons[1], (vec2_t){SCREEN_WIDTH / 2, 20}, (vec2_t){100, 25}, C2D_Color32f(1.f, 0.f, 0.f, 1.f), true);
    Button_InitVec2(&button_layout->buttons[2], (vec2_t){SCREEN_WIDTH / 2, 70}, (vec2_t){100, 25}, C2D_Color32f(0.f, 1.f, 0.f, 1.f), true);
    Button_InitVec2(&button_layout->buttons[3], (vec2_t){SCREEN_WIDTH / 2, 120}, (vec2_t){100, 25}, C2D_Color32f(0.f, 0.f, 1.f, 1.f), true);

    button_layout->buttons[1].on_pressed = Button_TestButton;
    button_layout->buttons[2].on_pressed = Button_TestButton;
    button_layout->buttons[3].on_pressed = Button_TestButton;
}

void ButtonLayout_Update(button_layout_t* button_layout) {
    for (u32 i = 0; i < LAYOUT_MAX_BUTTON_COUNT; i++) {
        button_t* current_button = &button_layout->buttons[i];
        if (current_button->initialized)
        {
            Button_Update(current_button);
        }
    }
}

void ButtonLayout_Draw(button_layout_t* button_layout) {
    for (u32 i = 0; i < LAYOUT_MAX_BUTTON_COUNT; i++) {
        button_t* current_button = &button_layout->buttons[i];
        if (current_button->initialized)
        {
            Button_Draw(current_button);
        }
    }
}
