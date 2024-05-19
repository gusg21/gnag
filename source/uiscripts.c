#include "uiscripts.h"

#include "_defs.h"
#include "game.h"

static game_t* UIScripts_S_Game = NULL;

void UIScripts_SetGame(game_t* game) { UIScripts_S_Game = game; }

void (*UIScripts_GetButtonCallbackByType(button_callback_type_e type))(button_t*) {
    switch (type) {
        case BUTTON_CALLBACK_NONE:
            return NULL;
        case BUTTON_CALLBACK_TEST:
            return UIScripts_TestButton;
        case BUTTON_CALLBACK_DEBUG_CONSOLE_TOGGLE:
            return NULL;
        case BUTTON_CALLBACK_MOVE:
            return UIScripts_Move;
        case BUTTON_CALLBACK_CONFIRM:
            return UIScripts_Confirm;

        default:
            return NULL;
    }
}

void (*UIScripts_GetButtonUpdaterByType(button_updater_type_e type))(button_t*) {
    switch (type) {
        case BUTTON_UPDATER_NONE:
            return NULL;

        default:
            return NULL;
    }
}

void (*UIScripts_GetFillBarUpdaterByType(fill_bar_updater_type_e type))(fill_bar_t*) {
    switch (type) {
        case FILL_BAR_UPDATER_NONE:
            return NULL;
        case FILL_BAR_UPDATER_HEALTH:
            return UIScripts_FillBarHealthUpdater;

        default:
            return NULL;
    }
}

void UIScripts_TestButton(button_t* button) {
    CTR_PRINTF("works\n");
}

void UIScripts_Move(button_t* button) {
    character_t* current = Board_GetCurrentActingCharacter(&UIScripts_S_Game->board);
    Board_EnqueuePlayerControlledCharacterAction(&UIScripts_S_Game->board,
                        (character_action_t){.character = current,
                                             .duration = 1.f,
                                             .initialized = true,
                                             .type = ACTION_MOVE,
                                             .move_source = current->pos,
                                             .move_destination = Vec2_Add(current->pos, (vec2_t){50.f, 0.f})});
}

void UIScripts_Confirm(button_t* button) {
    if (Board_HaveAllPlayerControlledCharactersActed(&UIScripts_S_Game->board)) {
        u32 actions_queued = Board_EnqueueAllPlayerControlledCharacterActionsToMainActionQueue(&UIScripts_S_Game->board);
        // todo)) @gusg21 DO AI
        Board_ExecuteQueue(&UIScripts_S_Game->board);
        Game_UpdateGameState(UIScripts_S_Game, GAME_STATE_PLAYER_ACTING);
        CTR_PRINTF("%ld actions queued\n", actions_queued);
    } else {
        CTR_PRINTF("selecting next char\n");
        Board_SelectNotYetActedCharacter(&UIScripts_S_Game->board);
    }
}

void UIScripts_FillBarHealthUpdater(fill_bar_t* fill_bar) {
    // Update no matter the state :)
    CTR_PRINTF("Fill bar update\n");
    FillBar_SetValue(fill_bar, Board_GetCurrentActingCharacter(&UIScripts_S_Game->board)->health);
}
