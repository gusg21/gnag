#include "characteraction.h"

#include "_defs.h"
#include "board.h"
#include "panic.h"

void CharacterAction_Run(character_action_t* action, board_t* board) {
    switch (action->type) {
        case CHARACTER_ACTION_MOVE: {
            // CTR_PRINTF("Move single character @ %p\n", action->character);
            character_t* target = action->character;
            float t = Board_GetNormalizedActionTime(board);
            if (!(t >= 1.0f)) {
                float move_count = action->tile_selections_count;

                int destination_index = (int)floorf(t * move_count);
                vec2_t destination = action->tile_selections[destination_index];
                vec2_t source =
                    destination_index <= 0 ? action->char_pos : action->tile_selections[destination_index - 1];
                float segment_t = (t * move_count) - destination_index;
                target->tile_pos = Vec2_Lerp(source, destination, segment_t);
            }

            break;
        }
        case CHARACTER_ACTION_CREATE_HAZARD: {
            float t = Board_GetNormalizedActionTime(board);
            if (!(t >= 1.0f)) {
                if (action->first_frame) {
                    action->first_frame = false;

                    for (u32 i = 0; i < action->tile_selections_count; i++) {
                        hazard_t* new_hazard = Board_NewHazard(board);
                        Hazard_Init(new_hazard, action->tile_selections[i].x, action->tile_selections[i].y, action->hazard_type);
                        CTR_PRINTF("splish splash\n");
                    }
                }
            }

            break;
        }
        case CHARACTER_ACTION_NONE: {
            break;
        }
        default:
            break;
    }
}
