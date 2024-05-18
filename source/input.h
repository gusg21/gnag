#ifndef INPUT_H
#define INPUT_H

#include <3ds.h>

#include "vec2.h"

bool Input_IsButtonDown(u32 button_mask);
bool Input_IsButtonPressed(u32 button_mask);
bool Input_IsButtonReleased(u32 button_mask);
bool Input_IsTouchScreenPressed();
bool Input_IsTouchScreenDown();
bool Input_IsTouchScreenReleased();
vec2_t Input_GetTouchPosition();

#endif // INPUT_H