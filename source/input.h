#ifndef INPUT_H
#define INPUT_H

#include <3ds.h>

bool Input_IsButtonDown(u32 button_mask);
bool Input_IsButtonPressed(u32 button_mask);
bool Input_IsButtonReleased(u32 button_mask);

#endif // INPUT_H