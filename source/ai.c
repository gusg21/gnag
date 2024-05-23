#include "ai.h"

#include "_defs.h"

void AI_Init(ai_t* ai) {
    UNUSED(ai); // todo)) remove once the ai param is actually used!
    
    // Stub
}

void AI_EnqueueAIActions(ai_t* ai, board_t* board) {
    UNUSED(ai); // todo)) remove once the ai param is actually used!

    for (u32 character_index = 0; character_index < BOARD_MAX_CHARACTER_COUNT; character_index++) {
        character_t* character = &board->characters[character_index];
        if (character->initialized && !character->is_player_controlled) {
            character_action_t action = (character_action_t){.type = CHARACTER_ACTION_MOVE,
                                                             .character = character,
                                                             .duration = 1.0f,
                                                             .initialized = true,
                                                             .char_pos = character->tile_pos,
                                                             .tile_selections_count = 2};
            action.tile_selections[0] = (vec2_t){character->tile_pos.x - 1, character->tile_pos.y};
            action.tile_selections[1] = (vec2_t){character->tile_pos.x, character->tile_pos.y - 1};
            Board_EnqueueAction(board, action);
        }
    }
}
