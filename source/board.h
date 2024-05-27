#ifndef BOARD_H
#define BOARD_H

#include "characteraction.h"
#include "character.h"
#include "grid.h"

#define BOARD_MAX_CHARACTER_COUNT 10
#define BOARD_MAX_CHARACTER_ACTION_QUEUE_LENGTH 128
#define BOARD_MAX_PLAYER_CONTROLLED_CHARACTER_COUNT 5 // must be < BOARD_MAX_CHARACTER_COUNT
#define BOARD_MAX_PLAYER_CONTROLLED_CHARACTER_ACTION_QUEUE_LENGTH 32
#define BOARD_MAX_HAZARDS 64

typedef struct board_s {
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
    s32 current_player_controlled_character_index;
    bool player_controlled_characters_acted_flags[BOARD_MAX_PLAYER_CONTROLLED_CHARACTER_COUNT];

    // Action queue w/ depth for PCCs
    ordered_character_action_t player_controlled_action_queue[BOARD_MAX_PLAYER_CONTROLLED_CHARACTER_ACTION_QUEUE_LENGTH];
    u32 next_player_controlled_action_index;
    u32 next_player_controlled_action_top_order;

    // Hazard data
    hazard_t hazards[BOARD_MAX_HAZARDS];
    u32 next_hazard_index;
} board_t;

// Loop functions
void Board_Init(board_t* board);
void Board_Update(board_t* board, float delta_time);
void Board_Draw(board_t* board, C2D_SpriteSheet sheet, grid_t* grid);

// Action Queue (primary)
void Board_EnqueueAction(board_t* board, character_action_t action);
void Board_ExecuteQueue(board_t* board);
void Board_ClearQueue(board_t* board);
float Board_GetNormalizedActionTime(board_t* board);
character_action_t* Board_GetCurrentAction(board_t* board);
character_t* Board_GetCurrentActingCharacter(board_t* board);

// PCCs
void Board_BuildPlayerControlledCharacterIndex(board_t* board);
void Board_EnqueuePlayerControlledCharacterAction(board_t* board, character_action_t action);
void Board_UndoLastPlayerControlledCharacterAction(board_t* board);
u32 Board_EnqueueAllPlayerControlledCharacterActionsToMainActionQueue(board_t* board);
character_t* Board_GetCurrentSelectedPlayerControlledCharacter(board_t* board);
u32 Board_GetPlayerControlledCharacterCount(board_t* board);
bool Board_HaveAllPlayerControlledCharactersActed(board_t* board);
void Board_SelectNotYetActedPlayerControlledCharacter(board_t* board);
void Board_SelectNextPlayerControlledCharacter(board_t* board);
void Board_SelectPreviousPlayerControlledCharacter(board_t* board);

// Character manip
character_t* Board_NewCharacter(board_t* board);
character_t* Board_GetCharacterByType(board_t* board, character_type_e type);
u32 Board_GetIndexByCharacter(board_t* board, character_t* character);

// Hazard manip
hazard_t* Board_NewHazard(board_t* board);

#endif // BOARD_H