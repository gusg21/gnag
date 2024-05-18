#include "board.h"

#include "panic.h"
#include "actionscripts.h"

void Board_Init(board_t* board) {
    memset(board->characters, 0, sizeof(character_t) * BOARD_MAX_CHARACTER_COUNT);
    memset(board->action_queue, 0, sizeof(character_action_t) * BOARD_MAX_CHARACTER_ACTION_QUEUE_LENGTH);

    board->next_character_index = 0;
    board->action_read_index = 0;
    board->action_write_index = 0;
    board->action_queue_executing = false;
    board->action_execution_time = 0.f;
}

void Board_Update(board_t* board, float delta_time) {
    if (board->action_queue_executing) {
        board->action_execution_time += delta_time;
        
        ActionScripts_Run(Board_GetCurrentAction(board), board);

        if (board->action_execution_time > Board_GetCurrentAction(board)->duration) {
            board->action_execution_time = 0.f;
            board->action_read_index++;

            if (!Board_GetCurrentAction(board)->initialized) {
                board->action_queue_executing = false;
            }
        }
    }
}

void Board_Draw(board_t* board, grid_t* grid) {
    for (u32 characterIndex = 0; characterIndex < BOARD_MAX_CHARACTER_COUNT; characterIndex++) {
        character_t* character = &board->characters[characterIndex];
        if (character->valid) Character_Draw(character);
    }
}

void Board_EnqueueAction(board_t* board, character_action_t action) {
    if (board->action_queue_executing) {
        Panic_Panic();
        printf("Tried to enqueue action while executing");
    }

    board->action_queue[board->action_write_index] = action;
    board->action_write_index++;
}

void Board_ExecuteQueue(board_t* board) {
    board->action_queue_executing = true;
    board->action_read_index = 0;
    board->action_execution_time = 0.f;
}

void Board_ClearQueue(board_t* board) {
    board->action_read_index = 0;
    board->action_write_index = 0;

    memset(board->action_queue, 0, sizeof(character_action_t) * BOARD_MAX_CHARACTER_ACTION_QUEUE_LENGTH);
}

float Board_GetNormalizedActionTime(board_t* board) {
    return board->action_execution_time / Board_GetCurrentAction(board)->duration;
}

character_action_t* Board_GetCurrentAction(board_t* board) { return &board->action_queue[board->action_read_index]; }

character_t* Board_NewCharacter(board_t* board) { 
    character_t* character = &board->characters[board->next_character_index];
    board->next_character_index++;
    return character;
}
