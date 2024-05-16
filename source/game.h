#ifndef GAME_H
#define GAME_H

#include <c2d/spritesheet.h>

#include "actor.h"

#define GAME_MAX_ACTORS 1024

typedef struct {
	C2D_SpriteSheet sheet;
	actor_t actors[GAME_MAX_ACTORS];
} game_t;

void Game_Init(game_t* game);
void Game_Destroy(game_t* game);
void Game_Draw(game_t* game);

#endif // GAME_H