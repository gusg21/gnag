#include "uiscripts.h"

#include "debugconsole.h"
#include "game.h"

static game_t* UIScripts_S_Game = NULL;

void UIScripts_SetGame(game_t* game) { UIScripts_S_Game = game; }

void (*UIScripts_GetFunctionByCallbackType(button_callback_type_e type))(button_t*) {
    switch (type) {
        case BUTTON_CALLBACK_NONE:
            return NULL;
        case BUTTON_CALLBACK_TEST:
            return Button_TestButton;
        case BUTTON_CALLBACK_DEBUG_CONSOLE_TOGGLE:
            return DebugConsole_ToggleEnabled;
        case BUTTON_CALLBACK_MOVE:
            return UIScripts_Move;
        case BUTTON_CALLBACK_CONFIRM:
            return UIScripts_Confirm;

        default:
            return NULL;
    }
}

void UIScripts_Move(button_t* button) {
    character_t* good = Board_GetCharacterByType(&UIScripts_S_Game->board, CHAR_GOOD);
    Board_EnqueueAction(&UIScripts_S_Game->board,
                        (character_action_t){.character = good,
                                             .duration = 1.f,
                                             .initialized = true,
                                             .type = ACTION_MOVE,
                                             .move_source = good->pos,
                                             .move_destination = Vec2_Add(good->pos, (vec2_t){50.f, 0.f})});
}

void UIScripts_Confirm(button_t* button) {
    Board_ExecuteQueue(&UIScripts_S_Game->board);
}
