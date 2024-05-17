#ifndef BOARD_H
#define BOARD_H

#include "character.h"
#include "grid.h"

#define BOARD_MAX_CHARACTER_COUNT 10

typedef struct {
    character_t characters[BOARD_MAX_CHARACTER_COUNT];

    u32 next_character_index;
} board_t;

void Board_Init(board_t* board);
void Board_Draw(board_t* board, grid_t* grid);
character_t* Board_NewCharacter(board_t* board);

#endif // BOARD_H