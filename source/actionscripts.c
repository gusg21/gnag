#include "actionscripts.h"

#include "board.h"

void ActionScripts_Run(character_action_t* action, board_t* board) {
    switch (action->type) {
        case ACTION_MOVE:
        {
            character_t* target = action->character;
            float t = Board_GetNormalizedActionTime(board);
            target->pos_x = Mathf_Lerp(action->move_source.x, action->move_destination.x, t);
            target->pos_y = Mathf_Lerp(action->move_source.y, action->move_destination.y, t);
        }
        case ACTION_NONE:
        {
            
        }
        break;
    }
}
