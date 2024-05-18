#ifndef GAME_H
#define GAME_H

#include <c2d/spritesheet.h>

#include "board.h"
#include "grid.h"

#define GAME_MAX_ACTORS 1024

typedef struct game_s {
	C2D_SpriteSheet sheet;
	grid_t grid;
	board_t board;
	float view_x, view_y;
} game_t;

void Game_Init(game_t* game);
void Game_Destroy(game_t* game);
void Game_Update(game_t* game, float delta_secs);
void Game_Draw(game_t* game);
character_t* Game_CreateCharacterAt(game_t* game, character_type_e type, u32 tile_x, u32 tile_y);

#endif // GAME_H