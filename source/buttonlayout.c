#include "buttonlayout.h"

#include <citro2d.h>

#include "_defs.h"
#include "debugconsole.h"

void ButtonLayout_Init(button_layout_t* button_layout) {
    memset(button_layout->buttons, 0, sizeof(button_t) * LAYOUT_MAX_BUTTON_COUNT);

    Button_InitVec2(&button_layout->buttons[0], (vec2_t){0, BOTTOM_SCREEN_HEIGHT - 10}, (vec2_t){10, 10}, C2D_Color32f(0.f, 0.f, 0.f, 1.f), true);

    button_layout->buttons[0].on_pressed = DebugConsole_ToggleEnabled;

    // Test buttons
    Button_InitVec2(&button_layout->buttons[1], (vec2_t){BOTTOM_SCREEN_WIDTH - 148, 20}, (vec2_t){96, 32}, C2D_Color32f(1.f, 0.5f, 0.5f, 1.f), true);
    Button_InitVec2(&button_layout->buttons[2], (vec2_t){BOTTOM_SCREEN_WIDTH - 148, 60}, (vec2_t){96, 32}, C2D_Color32f(0.5f, 1.f, 0.5f, 1.f), true);
    Button_InitVec2(&button_layout->buttons[3], (vec2_t){BOTTOM_SCREEN_WIDTH - 148, 100}, (vec2_t){96, 32}, C2D_Color32f(0.5f, 0.5f, 1.f, 1.f), true);

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
