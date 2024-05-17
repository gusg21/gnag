#include "board.h"

void Board_Init(board_t* board) {
    memset(board->characters, 0, sizeof(character_t) * BOARD_MAX_CHARACTER_COUNT);

    board->next_character_index = 0;
}

void Board_Draw(board_t* board, grid_t* grid) {
    for (u32 characterIndex = 0; characterIndex < BOARD_MAX_CHARACTER_COUNT; characterIndex++) {
        character_t* character = &board->characters[characterIndex];
        if (character->valid) Character_Draw(character);
    }
}

character_t* Board_NewCharacter(board_t* board) { 
    character_t* character = &board->characters[board->next_character_index];
    board->next_character_index++;
    return character;
}
