#include "characteraction.h"

#include "board.h"
#include "panic.h"

void CharacterAction_Run(character_action_t* action, board_t* board) {
    switch (action->type) {
        case ACTION_MOVE: {
            action->character->health -= 1.f;
            character_t* target = action->character;
            float t = Board_GetNormalizedActionTime(board);
            target->pos = Vec2_Lerp(action->move_source, action->move_destination, t);
            break;
        }
        case ACTION_NONE: {
            break;
        }
        default:
            break;
    }
}
