#ifndef AI_H
#define AI_H

#include "board.h"

typedef struct ai_s {
    // AI state goes here, any kind of persistent thoughts the system would like to hold on to.
    char __placeholder__;
} ai_t;

void AI_Init(ai_t* ai);
void AI_EnqueueAIActions(ai_t* ai, board_t* board);

#endif // AI_H