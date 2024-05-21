#include "ai.h"

void AI_Init(ai_t* ai) {
    // Stub
}

void AI_EnqueueAIActions(ai_t* ai, board_t* board) {
    for (u32 character_index = 0; character_index < BOARD_MAX_CHARACTER_COUNT; character_index++) {
        character_t* character = &board->characters[character_index];
        if (character->initialized && !character->is_player_controlled) {
            character_action_t action = (character_action_t){.type = CHARACTER_ACTION_MOVE,
                                                             .character = character,
                                                             .duration = 1.0f,
                                                             .initialized = true,
                                                             .move_source = character->tile_pos,
                                                             .move_destination_count = 2};
            action.move_destinations[0] = (vec2_t){character->tile_pos.x - 1, character->tile_pos.y};
            action.move_destinations[1] = (vec2_t){character->tile_pos.x, character->tile_pos.y - 1};
            Board_EnqueueAction(board, action);
        }
    }
}
