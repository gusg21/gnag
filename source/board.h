#ifndef BOARD_H
#define BOARD_H

#include "character.h"
#include "grid.h"

#define BOARD_MAX_CHARACTER_COUNT 10
#define BOARD_MAX_CHARACTER_ACTION_QUEUE_LENGTH 128
#define BOARD_MAX_PLAYER_CONTROLLED_CHARACTER_COUNT 5 // must be < BOARD_MAX_CHARACTER_COUNT
#define BOARD_MAX_PLAYER_CONTROLLED_CHARACTER_ACTION_QUEUE_LENGTH 32

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
    bool initialized;
    character_action_t action;
    u32 order;
} ordered_character_action_t;

typedef struct {
    // All the characters
    character_t characters[BOARD_MAX_CHARACTER_COUNT];
    u32 next_character_index;

    // Full action queue
    character_action_t action_queue[BOARD_MAX_CHARACTER_ACTION_QUEUE_LENGTH];
    u32 action_write_index, action_read_index;
    float action_execution_secs;
    bool action_queue_executing;

    // PCCs
    character_t* player_controlled_characters[BOARD_MAX_PLAYER_CONTROLLED_CHARACTER_COUNT];
    u32 current_player_controlled_character_index;
    bool player_controlled_characters_acted_flags[BOARD_MAX_PLAYER_CONTROLLED_CHARACTER_COUNT];

    // Action queue w/ depth for PCCs
    ordered_character_action_t player_controlled_action_queue[BOARD_MAX_PLAYER_CONTROLLED_CHARACTER_ACTION_QUEUE_LENGTH];
    u32 next_player_controlled_action_index;
    u32 next_player_controlled_action_top_order;
} board_t;

void Board_Init(board_t* board);
void Board_Update(board_t* board, float delta_time);
void Board_Draw(board_t* board, grid_t* grid);
void Board_BuildPlayerControlledCharacterIndex(board_t* board);
void Board_EnqueueAction(board_t* board, character_action_t action);
void Board_EnqueuePlayerControlledCharacterAction(board_t* board, character_action_t action);
void Board_EnqueueAllPlayerControlledCharacterActionsToMainActionQueue(board_t* board);
void Board_ExecuteQueue(board_t* board);
void Board_ClearQueue(board_t* board);
float Board_GetNormalizedActionTime(board_t* board);
character_action_t* Board_GetCurrentAction(board_t* board);
character_t* Board_GetCurrentActingCharacter(board_t* board);
void Board_SelectNotYetActedCharacter(board_t* board);
bool Board_HaveAllPlayerControlledCharactersActed(board_t* board);
character_t* Board_NewCharacter(board_t* board);
character_t* Board_GetCharacterByType(board_t* board, character_type_e type);

#endif // BOARD_H