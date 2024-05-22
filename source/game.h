#ifndef GAME_H
#define GAME_H

#include <c2d/spritesheet.h>

#include "board.h"
#include "grid.h"
#include "ai.h"

#define GAME_MAX_ACTORS 1024

typedef enum {
	GAME_STATE_NONE, // Should never be this unless very specific case
	GAME_STATE_PAUSED, // Ingame pause or 3DS on homescreen
	GAME_STATE_PLAYER_TURN, // Input allowed between enemy moves end and player moves complete
	GAME_STATE_PLAYER_ACTING, // Player characters completing queued actions and environment updating
	GAME_STATE_OPPONENT_TURN, // Waiting for enemy moves to complete (SHOULD BE QUICK, AI & ANIMATION)
	GAME_STATE_OPPONENT_ACTING, // Enemy characters completing actions and environment updating
	GAME_STATE_SELECTING_TILE, // Selecting tile to target on top screen

} game_state_e;

typedef struct game_s {
	game_state_e state;

	C2D_SpriteSheet sheet;
	grid_t grid;
	board_t board;
	ai_t ai;
	vec2_t view_pos;
	vec2_t focus_pos;

	vec2_t* selected_tiles; 
	vec2_t selected_tile_pos;

} game_t;

void Game_Init(game_t* game);
void Game_Destroy(game_t* game);
void Game_Update(game_t* game, float delta_secs);
void Game_Draw(game_t* game);

void Game_DoPlayerTurn(game_t* game);
void Game_DoPlayerActing(game_t* game);
void Game_DoOpponentTurn(game_t* game);
void Game_DoOpponentActing(game_t* game);
void Game_DoSelectingTile(game_t* game);

character_t* Game_CreateCharacterAt(game_t* game, character_type_e type, bool is_player_controlled, float tile_x, float tile_y);

bool Game_IsValidTileSelection(game_t* game);
void Game_UpdateSelectedTiles(game_t* game);
void Game_UpdateGameState(game_t* game, game_state_e state);

#endif // GAME_H