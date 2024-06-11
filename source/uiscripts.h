#ifndef UI_SCRIPTS_H
#define UI_SCRIPTS_H

#include "button.h"
#include "fillbar.h"
#include "text.h"
#include "image.h"

typedef struct game_s game_t;

void UIScripts_SetGame(game_t* game);
void (*UIScripts_GetButtonCallbackByType(button_callback_type_e type))(button_t*);
void (*UIScripts_GetButtonUpdaterByType(button_updater_type_e type))(button_t*);
void (*UIScripts_GetFillBarUpdaterByType(fill_bar_updater_type_e type))(fill_bar_t*);
void (*UIScripts_GetImageUpdaterByType(image_updater_type_e type))(image_t*);
void (*UIScripts_GetTextUpdaterByType(text_updater_type_e type))(text_t*);

// Callbacks
void UIScripts_LineOfFire(button_t* button);
void UIScripts_LineOfWater(button_t* button);
void UIScripts_LineOfLightning(button_t* button);
void UIScripts_Gust(button_t* button);
void UIScripts_Move(button_t* button);
void UIScripts_Confirm(button_t* button);
void UIScripts_NextPlayerCharacter(button_t* button);
void UIScripts_PrevPlayerCharacter(button_t* button);

// Updaters
void UIScripts_ButtonPlayerTurnUpdater(button_t* button);
void UIScripts_ButtonMoveUpdater(button_t* button);
void UIScripts_ButtonConfirmUpdater(button_t* button);
void UIScripts_FillBarHealthUpdater(fill_bar_t* fill_bar);
void UIScripts_FillBarMovementUpdater(fill_bar_t* fill_bar);
void UIScripts_TextSelectedCharacterNameUpdater(text_t* text);

#endif // UI_SCRIPTS_H