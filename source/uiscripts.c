#include "uiscripts.h"

#include "_defs.h"
#include "game.h"

static game_t* UIScripts_S_Game = NULL;

void UIScripts_SetGame(game_t* game) { UIScripts_S_Game = game; }

void (*UIScripts_GetButtonCallbackByType(button_callback_type_e type))(button_t*) {
    switch (type) {
        case BUTTON_CALLBACK_NONE:
            return NULL;
        case BUTTON_CALLBACK_MOVE:
            return UIScripts_Move;
        case BUTTON_CALLBACK_CONFIRM:
            return UIScripts_Confirm;
        case BUTTON_CALLBACK_PREV_CHARACTER:
            return UIScripts_PrevPlayerCharacter;
        case BUTTON_CALLBACK_NEXT_CHARACTER:
            return UIScripts_NextPlayerCharacter;
        case BUTTON_CALLBACK_CUBEOFWATER:
            return UIScripts_CubeOfWater;
        case BUTTON_CALLBACK_LINEOFSPIKES:
            return UIScripts_LineOfSpikes;

        default:
            return NULL;
    }
}

void (*UIScripts_GetButtonUpdaterByType(button_updater_type_e type))(button_t*) {
    switch (type) {
        case BUTTON_UPDATER_NONE:
            return NULL;
        case BUTTON_UPDATER_PLAYER_TURN:
            return UIScripts_ButtonPlayerTurnUpdater;
        case BUTTON_UPDATER_MOVE:
            return UIScripts_ButtonMoveUpdater;
        case BUTTON_UPDATER_CONFIRM:
            return UIScripts_ButtonConfirmUpdater;

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
        case FILL_BAR_UPDATER_MOVEMENT:
            return UIScripts_FillBarMovementUpdater;

        default:
            return NULL;
    }
}

void (*UIScripts_GetImageUpdaterByType(image_updater_type_e type))(image_t*) {
    switch (type) {
        case IMAGE_UPDATER_NONE:
            return NULL;
        // Enemy themed BG vs player themed BG? Character themed BG?
        case IMAGE_UPDATER_INFO:
            return NULL;

        default:
            return NULL;
    }
}

void (*UIScripts_GetTextUpdaterByType(text_updater_type_e type))(text_t*) {
    switch (type) {
        case TEXT_UPDATER_NONE:
            return NULL;
        case TEXT_UPDATER_SELECTED_CHARACTER_NAME:
            return UIScripts_TextSelectedCharacterNameUpdater;

        default:
            return NULL;
    }
}

void UIScripts_CubeOfWater(button_t* button) {
    UNUSED(button);
    Game_UpdateGameState(UIScripts_S_Game, GAME_STATE_SELECTING_TILE);
    Game_UpdateSelectionType(UIScripts_S_Game, SELECTING_TILE_LINE, 3, 3);
    Game_UpdateSelectionHazardType(UIScripts_S_Game, HAZARD_WATER);
}

void UIScripts_LineOfSpikes(button_t* button) {
    UNUSED(button);
    Game_UpdateGameState(UIScripts_S_Game, GAME_STATE_SELECTING_TILE);
    Game_UpdateSelectionType(UIScripts_S_Game, SELECTING_TILE_LINE, 4, 1);
    Game_UpdateSelectionHazardType(UIScripts_S_Game, HAZARD_SPIKES);
}

void UIScripts_Move(button_t* button) {
    UNUSED(button);

    Game_UpdateGameState(UIScripts_S_Game, GAME_STATE_SELECTING_TILE);
    Game_UpdateSelectionType(UIScripts_S_Game, SELECTING_TILE_MOVE,
                             Board_GetCurrentSelectedPlayerControlledCharacter(&UIScripts_S_Game->board)->move_speed,
                             1);
}

void UIScripts_Confirm(button_t* button) {
    UNUSED(button);
    UIScripts_S_Game->board
        .player_controlled_characters_acted_flags[UIScripts_S_Game->board.current_player_controlled_character_index] =
        true;
    if (Board_HaveAllPlayerControlledCharactersActed(&UIScripts_S_Game->board)) {
        u32 actions_queued =
            Board_EnqueueAllPlayerControlledCharacterActionsToMainActionQueue(&UIScripts_S_Game->board);
        // todo)) @gusg21 DO AI
        Game_UpdateGameState(UIScripts_S_Game, GAME_STATE_PLAYER_ACTING);
        Board_ExecuteQueue(&UIScripts_S_Game->board);
        CTR_PRINTF("%ld actions queued\n", actions_queued);
    } else {
        CTR_PRINTF("selecting next char\n");
        Board_SelectNotYetActedPlayerControlledCharacter(&UIScripts_S_Game->board);
    }
}

void UIScripts_NextPlayerCharacter(button_t* button) {
    UNUSED(button);
    Board_SelectNextPlayerControlledCharacter(&UIScripts_S_Game->board);
}

void UIScripts_PrevPlayerCharacter(button_t* button) {
    UNUSED(button);
    Board_SelectPreviousPlayerControlledCharacter(&UIScripts_S_Game->board);
}

void UIScripts_ButtonPlayerTurnUpdater(button_t* button) {
    if (UIScripts_S_Game->state == GAME_STATE_PLAYER_TURN) {
        button->pressable = true;
    } else {
        button->pressable = false;
    }
}

void UIScripts_ButtonMoveUpdater(button_t* button) {
    if (UIScripts_S_Game->state == GAME_STATE_PLAYER_TURN &&
        !Board_GetCurrentSelectedPlayerControlledCharacter(&UIScripts_S_Game->board)->moved) {
        button->pressable = true;
    } else {
        button->pressable = false;
    }
}

void UIScripts_ButtonConfirmUpdater(button_t* button) {
    if (UIScripts_S_Game->state == GAME_STATE_PLAYER_TURN) {
        button->pressable = true;
    } else {
        button->pressable = false;
    }
}

void UIScripts_FillBarHealthUpdater(fill_bar_t* fill_bar) {
    // Update no matter the state :)
    FillBar_SetValue(fill_bar, Board_GetCurrentSelectedPlayerControlledCharacter(&UIScripts_S_Game->board)->health);
}

void UIScripts_FillBarMovementUpdater(fill_bar_t* fill_bar) {
    FillBar_SetValue(fill_bar, Board_GetCurrentSelectedPlayerControlledCharacter(&UIScripts_S_Game->board)->move_speed -
                                   UIScripts_S_Game->current_tile_index);
}

void UIScripts_TextSelectedCharacterNameUpdater(text_t* text) {
    // todo)) @gusg21 this is expensive to do every frame
    Text_SetText(text, CharacterData_GetName(Board_GetCurrentSelectedPlayerControlledCharacter(&UIScripts_S_Game->board)->type));
}
