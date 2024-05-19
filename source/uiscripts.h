#ifndef UI_SCRIPTS_H
#define UI_SCRIPTS_H

#include "button.h"
#include "fillbar.h"

typedef struct game_s game_t;

void UIScripts_SetGame(game_t* game);
void (*UIScripts_GetButtonCallbackByType(button_callback_type_e type))(button_t*);
void (*UIScripts_GetButtonUpdaterByType(button_updater_type_e type))(button_t*);
void (*UIScripts_GetFillBarUpdaterByType(fill_bar_updater_type_e type))(fill_bar_t*);

// Callbacks
void UIScripts_TestButton(button_t* button);
void UIScripts_Move(button_t* button);
void UIScripts_Confirm(button_t* button);

// Updaters
void UIScripts_FillBarHealthUpdater(fill_bar_t* fill_bar);

#endif // UI_SCRIPTS_H