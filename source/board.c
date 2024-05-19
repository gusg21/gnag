#include "board.h"

#include "panic.h"
#include "actionscripts.h"
#include "debugconsole.h"

void Board_Init(board_t* board) {
    memset(board->characters, 0, sizeof(character_t) * BOARD_MAX_CHARACTER_COUNT);
    memset(board->action_queue, 0, sizeof(character_action_t) * BOARD_MAX_CHARACTER_ACTION_QUEUE_LENGTH);
    memset(board->player_controlled_characters, 0, sizeof(character_t*) * BOARD_MAX_PLAYER_CONTROLLED_CHARACTER_COUNT);
    memset(board->player_controlled_characters_acted_flags, 0, sizeof(bool) * BOARD_MAX_PLAYER_CONTROLLED_CHARACTER_COUNT);
    memset(board->player_controlled_action_queue, 0, sizeof(ordered_character_action_t) * BOARD_MAX_PLAYER_CONTROLLED_CHARACTER_ACTION_QUEUE_LENGTH);

    board->next_character_index = 0;
    board->action_read_index = 0;
    board->action_write_index = 0;
    board->action_queue_executing = false;
    board->action_execution_secs = 0.f;

    board->current_player_controlled_character_index = 0;

    board->next_player_controlled_action_index = 0;
    board->next_player_controlled_action_top_order = 0;
}

void Board_Update(board_t* board, float delta_secs) {
    if (board->action_queue_executing) {
        board->action_execution_secs += delta_secs;
        
        if (Board_GetNormalizedActionTime(board) >= 1.f) {
            board->action_execution_secs = 0.f;
            board->action_read_index++;

            if (!Board_GetCurrentAction(board)->initialized) {
                board->action_queue_executing = false;
                Board_ClearQueue(board);
                DebugConsole_Print("end acting", 11);
            } else {
                DebugConsole_Print("next action", 12);
            }
        }

        ActionScripts_Run(Board_GetCurrentAction(board), board);
    }
}

void Board_Draw(board_t* board, grid_t* grid) {
    for (u32 characterIndex = 0; characterIndex < BOARD_MAX_CHARACTER_COUNT; characterIndex++) {
        character_t* character = &board->characters[characterIndex];
        if (character->initialized) Character_Draw(character);
    }
}

void Board_BuildPlayerControlledCharacterIndex(board_t* board) {
    u32 next_pcc_slot = 0; // PCC = player controlled character
    for (u32 character_index = 0; character_index < BOARD_MAX_CHARACTER_COUNT; character_index++) {
        character_t* character = &board->characters[character_index];
        if (character->is_player_controlled) {
            board->player_controlled_characters[next_pcc_slot] = character;
            next_pcc_slot++;
        }
    }
}

void Board_EnqueueAction(board_t* board, character_action_t action) {
    if (board->action_queue_executing) {
        Panic_Panic();
        printf("Tried to enqueue action while executing");
    }

    DebugConsole_Print("action enqueued", 16);

    board->action_queue[board->action_write_index] = action;
    board->action_queue[board->action_write_index].initialized = true;
    board->action_write_index++;
}

void Board_EnqueuePlayerControlledCharacterAction(board_t* board, character_action_t action) {
    // Add this to the PCC queue
    board->player_controlled_action_queue[board->next_player_controlled_action_index] = (ordered_character_action_t) {
        .action = action,
        .initialized = true,
        .order = board->next_player_controlled_action_top_order
    };

    DebugConsole_PrintInt(board->next_player_controlled_action_top_order);

    // Mark this character as having acted
    board->player_controlled_characters_acted_flags[board->current_player_controlled_character_index] = true;

    // Update the indices and the next order to give
    board->next_player_controlled_action_index++;
    board->next_player_controlled_action_top_order++;
}

void Board_EnqueueAllPlayerControlledCharacterActionsToMainActionQueue(board_t* board) {
    ordered_character_action_t* lowest;
    u32 lowest_order;

    while (true) {
        lowest = NULL;
        lowest_order = -10000; // We're realists here

        // Find the lowest ordered action
        for (u32 action_index = 0; action_index < BOARD_MAX_PLAYER_CONTROLLED_CHARACTER_ACTION_QUEUE_LENGTH; action_index++) {
            ordered_character_action_t* action = &board->player_controlled_action_queue[action_index];
            if (action->initialized) {
                if (action->order > lowest_order) {
                    lowest_order = action->order;
                    lowest = action;
                }
            }
        }

        if (lowest != NULL) {
            // Remove it from the queue
            lowest->initialized = false;

            // Add it to the main queue
            Board_EnqueueAction(board, lowest->action);
        } else {
            // The queue is empty now, move on
            break;
        }
    }

    // Reset the queue + state
    memset(board->player_controlled_action_queue, 0, sizeof(ordered_character_action_t) * BOARD_MAX_PLAYER_CONTROLLED_CHARACTER_ACTION_QUEUE_LENGTH);

    board->next_player_controlled_action_index = 0;
    board->next_player_controlled_action_top_order = 0;
}

void Board_ExecuteQueue(board_t* board) {
    board->action_queue_executing = true;
    board->action_read_index = 0;
    board->action_execution_secs = 0.f;
}

void Board_ClearQueue(board_t* board) {
    board->action_read_index = 0;
    board->action_write_index = 0;

    memset(board->action_queue, 0, sizeof(character_action_t) * BOARD_MAX_CHARACTER_ACTION_QUEUE_LENGTH);
}

float Board_GetNormalizedActionTime(board_t* board) {
    float t = board->action_execution_secs / Board_GetCurrentAction(board)->duration;

    return Mathf_Clamp(t, 0.f, 1.f);
}

character_action_t* Board_GetCurrentAction(board_t* board) { return &board->action_queue[board->action_read_index]; }

character_t* Board_GetCurrentActingCharacter(board_t* board) {
    return board->player_controlled_characters[board->current_player_controlled_character_index];
}

void Board_SelectNotYetActedCharacter(board_t* board) {
    for (u32 index = 0; index < BOARD_MAX_PLAYER_CONTROLLED_CHARACTER_COUNT; index++) {
        // if exists a PCC that's actually real (non-NULL) and also has not acted, select it
        if (board->player_controlled_characters[index] != NULL && !board->player_controlled_characters_acted_flags[index] == true) {
            DebugConsole_Print("Selecting new character", 24);
            board->current_player_controlled_character_index = index;
            break;
        }
    }
}

bool Board_HaveAllPlayerControlledCharactersActed(board_t* board) {
    for (u32 index = 0; index < BOARD_MAX_PLAYER_CONTROLLED_CHARACTER_COUNT; index++) {
        // if exists a PCC that's actually real (non-NULL) and also has not acted, we're not done yet
        if (board->player_controlled_characters[index] != NULL && !board->player_controlled_characters_acted_flags[index]) {
            return false;
        }
    }
    return true;
}

character_t* Board_NewCharacter(board_t* board) { 
    character_t* character = &board->characters[board->next_character_index];
    board->next_character_index++;
    return character;
}

character_t* Board_GetCharacterByType(board_t* board, character_type_e type) {
    for (u32 character_index = 0; character_index < BOARD_MAX_CHARACTER_COUNT; character_index++) {
        character_t* character = &board->characters[character_index];
        if (character->type == type && character->initialized) {
            return character;
        }
    }
    
    // Freak out!
    Panic_Panic();
    printf("no character with requested type %d.\n", type);
    printf("found types: ");
    for (u32 character_index = 0; character_index < BOARD_MAX_CHARACTER_COUNT; character_index++) {
        character_t* character = &board->characters[character_index];
        if (character->initialized) {
            printf("%d ", character->type);
        }
    }
    return NULL;
}
