#include "input.h"

bool Input_IsButtonDown(u32 button_mask) {
    u32 keys_down = hidKeysHeld();

    return keys_down & button_mask;
}

bool Input_IsButtonPressed(u32 button_mask) {
    u32 keys_pressed = hidKeysDown();

    return keys_pressed & button_mask;
}

bool Input_IsButtonReleased(u32 button_mask) {
    u32 keys_released = hidKeysUp();

    return keys_released & button_mask;
}

bool Input_IsTouchScreenPressed() { return Input_IsButtonPressed(KEY_TOUCH); }

bool Input_IsTouchScreenDown() { return Input_IsButtonDown(KEY_TOUCH); }

bool Input_IsTouchScreenReleased() { return Input_IsButtonReleased(KEY_TOUCH); }

vec2_t Input_GetTouchPosition() {
    touchPosition touch_pos;
    hidTouchRead(&touch_pos);
    return (vec2_t){.x = touch_pos.px, .y = touch_pos.py};
}
