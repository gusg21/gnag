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
