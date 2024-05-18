#ifndef BOARD_H
#define BOARD_H

#include "character.h"
#include "grid.h"

#define BOARD_MAX_CHARACTER_COUNT 10
#define BOARD_MAX_CHARACTER_ACTION_QUEUE_LENGTH 128

typedef enum {
    ACTION_NONE, // Nothing
    ACTION_MOVE // Move the character to a position
} character_action_type_e;

typedef struct {
    bool initialized;

    character_action_type_e type;
    character_t* character;
    float duration;

    vec2_t move_source;
    vec2_t move_destination;
} character_action_t;

typedef struct {
    character_t characters[BOARD_MAX_CHARACTER_COUNT];
    u32 next_character_index;

    character_action_t action_queue[BOARD_MAX_CHARACTER_ACTION_QUEUE_LENGTH];
    u32 action_write_index, action_read_index;
    float action_execution_time;
    bool action_queue_executing;
} board_t;

void Board_Init(board_t* board);
void Board_Update(board_t* board, float delta_time);
void Board_Draw(board_t* board, grid_t* grid);
void Board_EnqueueAction(board_t* board, character_action_t action);
void Board_ExecuteQueue(board_t* board);
void Board_ClearQueue(board_t* board);
float Board_GetNormalizedActionTime(board_t* board);
character_action_t* Board_GetCurrentAction(board_t* board);
character_t* Board_NewCharacter(board_t* board);

#endif // BOARD_H