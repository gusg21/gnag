#ifndef UI_SCRIPTS_H
#define UI_SCRIPTS_H

#include "button.h"
#include "fillbar.h"

typedef struct game_s game_t;

void UIScripts_SetGame(game_t* game);
void (*UIScripts_GetFunctionByCallbackType(button_callback_type_e type))(button_t*);

void UIScripts_UpdateButton(button_t* button);
void UIScripts_UpdateFillBar(fill_bar_t* fill_bar);

void UIScripts_TestButton(button_t* button);
void UIScripts_Move(button_t* button);
void UIScripts_Confirm(button_t* button);

#endif // UI_SCRIPTS_H