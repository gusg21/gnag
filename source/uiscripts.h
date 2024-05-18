#ifndef UI_SCRIPTS_H
#define UI_SCRIPTS_H

#include "button.h"

void (*UIScripts_GetFunctionByCallbackType(ui_callback_type_e type))(button_t*);

void UIScripts_Move(button_t* button);

#endif // UI_SCRIPTS_H