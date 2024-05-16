// Simple citro2d sprite drawing example
// Images borrowed from:
//   https://kenney.nl/assets/space-shooter-redux

#include <citro2d.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#include "game.h"
#include "sprites.h"

#define SCREEN_WIDTH  400
#define SCREEN_HEIGHT 240

//---------------------------------------------------------------------------------
int main() {
//---------------------------------------------------------------------------------
	// Init libs
	romfsInit();
	gfxInitDefault();
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();

	// Create screens
	C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);

	// Allocate game data
	game_t* game = malloc(sizeof(game_t));
	memset(game, 0, sizeof(game_t));

	// Init game
	Game_Init(game);

	// Try to load tile
	C2D_Image set = C2D_SpriteSheetGetImage(game->sheet, sprites_tileset_idx);

	// Main loop
	while (aptMainLoop())
	{
		hidScanInput();

		u32 kHeld = hidKeysHeld();

		// Render the scene
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_TargetClear(top, C2D_Color32f(0.0f, 0.0f, 0.0f, 1.0f));
		C2D_SceneBegin(top);
		{
			
			Game_Draw(game);
		}
		C3D_FrameEnd(0);
	}

	// Delete the game
	Game_Destroy(game);
	free(game);
	game = NULL;

	// Deinit libs
	C2D_Fini();
	C3D_Fini();
	gfxExit();
	romfsExit();
	return 0;
}
