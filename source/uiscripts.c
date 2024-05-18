#include "uiscripts.h"

#include "debugconsole.h"

void (*UIScripts_GetFunctionByCallbackType(ui_callback_type_e type))(button_t*) {
    switch (type) {
        case UI_ACTION_NONE:
            return NULL;
        case UI_ACTION_TEST:
            return Button_TestButton;
        case UI_ACTION_DEBUG_CONSOLE_TOGGLE:
            return DebugConsole_ToggleEnabled;
        case UI_ACTION_MOVE:
            return UIScripts_Move;

        default:
            return NULL;
    }
}

void UIScripts_Move(button_t* button) {}
