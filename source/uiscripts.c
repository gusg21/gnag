#include "uiscripts.h"

#include "_defs.h"
#include "game.h"

static game_t* UIScripts_S_Game = NULL;

void UIScripts_SetGame(game_t* game) { UIScripts_S_Game = game; }

void (*UIScripts_GetFunctionByCallbackType(button_callback_type_e type))(button_t*) {
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

void UIScripts_UpdateButton(button_t* button) {
    switch (UIScripts_S_Game->state)
    {
        case GAME_STATE_PAUSED:
            button->pressable = false;
            return;
        case GAME_STATE_PLAYER_TURN:
            button->pressable = true;
            return;
        case GAME_STATE_PLAYER_ACTING:
            button->pressable = false;
            return;
        case GAME_STATE_OPPONENT_TURN:
            button->pressable = false;
            return;
        case GAME_STATE_OPPONENT_ACTING:
            button->pressable = false;
            return;
        case GAME_STATE_SELECTING_TILE:
            button->pressable = false;
            return;
        default:
            return; //do nothing
    }
}

void UIScripts_UpdateFillBar(fill_bar_t* fill_bar) {
    switch (UIScripts_S_Game->state)
    {
        case GAME_STATE_PAUSED:
            fill_bar->is_updating = false;
            return;
        case GAME_STATE_PLAYER_TURN:
            fill_bar->is_updating = true;
            return;
        case GAME_STATE_PLAYER_ACTING:
            fill_bar->is_updating = true;
            return;
        case GAME_STATE_OPPONENT_TURN:
            fill_bar->is_updating = false;
            return;
        case GAME_STATE_OPPONENT_ACTING:
            fill_bar->is_updating = true;
            return;
        case GAME_STATE_SELECTING_TILE:
            fill_bar->is_updating = true;
            return;
        default:
            return; //do nothing
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
