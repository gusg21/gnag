// Simple citro2d sprite drawing example
// Images borrowed from:
//   https://kenney.nl/assets/space-shooter-redux

#include <3ds.h>
#include <citro2d.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "_defs.h"
#include "button.h"
#include "game.h"
#include "input.h"
#include "panic.h"
#include "ui.h"
#include "sprites.h"

int main() {
    // Init libs
    romfsInit();
    cfguInit();
    gfxInitDefault();
    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
    C2D_Prepare();

    // Pipe printf to SVC call
    consoleDebugInit(debugDevice_SVC);

    // Create screens
    C3D_RenderTarget* top_screen = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
    C3D_RenderTarget* bottom_screen = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);

    // Allocate game data
    game_t* game = malloc(sizeof(game_t));
    memset(game, 0, sizeof(game_t));

    // Init the game
    Game_Init(game);

	// Put in testing characters; will eventually be loaded from a scenario struct
    Game_CreateCharacterAt(game, CHAR_GOOD, true, 5, 5);
    Game_CreateCharacterAt(game, CHAR_BAD, true, 2, 3);
    Board_BuildPlayerControlledCharacterIndex(&game->board);
    Grid_GetTileAt(&game->grid, 0, 0)->is_spikes = true;
	Grid_Build(&game->grid);
    
	// Create and Init Bottom Screen UI
    ui_layout_t button_layout;
	ui_t bottom_ui;
    UILayout_InitFromFile(&button_layout, "romfs:/jsons/uilayout_playerturn.json");
	UI_Init(&bottom_ui, game, &button_layout);

	// Panic_Panic();
	// printf("Test panic");

	u32 frame_num = 0;

    // Main loop
    while (aptMainLoop() && !Panic_IsPanicked()) {
        hidScanInput();

        float delta_secs = 1.f / 60.f;

        Game_Update(game, delta_secs);  // Fixed timestep for now
        UI_Update(&bottom_ui);

        // Render the scene
        C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
        C2D_TargetClear(top_screen, C2D_Color32(223, 246, 245, 255));
        C2D_SceneBegin(top_screen);
        { Game_Draw(game); }

        C2D_TargetClear(bottom_screen, C2D_Color32(223, 246, 245, 255));
        C2D_SceneBegin(bottom_screen);
        {
            // Bottom screen drawing
            UI_Draw(&bottom_ui);
        }
        C3D_FrameEnd(0);

        frame_num++;
    }

    // Panic Loop
    if (Panic_IsPanicked()) {
        while (aptMainLoop()) {
            hidScanInput();

            C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
            C2D_TargetClear(top_screen, C2D_Color32(223, 246, 245, 255));
            C2D_TargetClear(bottom_screen, C2D_Color32(223, 246, 245, 255));
            C3D_FrameEnd(0);
        }
    }

    // Delete the game
    Game_Destroy(game);
    free(game);
    game = NULL;

    UI_Destroy(&bottom_ui);

    // Deinit libs
    C2D_Fini();
    C3D_Fini();
    gfxExit();
    romfsExit();
    return 0;
}
