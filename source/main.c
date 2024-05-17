// Simple citro2d sprite drawing example
// Images borrowed from:
//   https://kenney.nl/assets/space-shooter-redux

#include <citro2d.h>
#include <3ds.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#include "_defs.h"
#include "game.h"
#include "sprites.h"
#include "input.h"
#include "debugconsole.h"

int main() {
	// Init libs
	romfsInit();
	cfguInit(); 
	gfxInitDefault();
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();

	// Create screens
	C3D_RenderTarget* top_screen = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
	C3D_RenderTarget* bottom_screen = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);

	// Create debug console
	DebugConsole_Init();
	DebugConsole_Print("DEBUG CONSOLE", 14);

	// Allocate game data
	game_t* game = malloc(sizeof(game_t));
	memset(game, 0, sizeof(game_t));

	// Init the game
	Game_Init(game);
	Game_CreateCharacterAt(game, CHAR_GOOD, 5, 5);
	Game_CreateCharacterAt(game, CHAR_BAD, 2, 3);
	Grid_GetTileAt(&game->grid, 0, 0)->is_spikes = true;
	
	DebugConsole_Print("game initialized", 17);

	u32 frame_num = 0;

	// Main loop
	while (aptMainLoop())
	{
		hidScanInput();

		Game_Update(game, 1.f / 60.f); // Fixed timestep for now

		// Render the scene
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_TargetClear(top_screen, C2D_Color32(223, 246, 245, 255));
		C2D_SceneBegin(top_screen);
		{
			Game_Draw(game);
		}

		C2D_TargetClear(bottom_screen, C2D_Color32(223, 246, 245, 255));
		C2D_SceneBegin(bottom_screen);
		{
			// Bottom screen drawing
			DebugConsole_Draw();
		}
		C3D_FrameEnd(0);

		frame_num++;
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
