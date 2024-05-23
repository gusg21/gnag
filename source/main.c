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

// Handle gnag_debug_handle;

int main() {

    // Debug Setup
    // svcDebugActiveProcess(&gnag_debug_handle, CUR_PROCESS_HANDLE);
    // svcBreakDebugProcess(gnag_debug_handle);

    // Pipe printf to SVC call
    consoleDebugInit(debugDevice_SVC);

    CTR_PRINTF("gnag main()\n");

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

    // Allocate game data
    game_t* game = malloc(sizeof(game_t));
    memset(game, 0, sizeof(game_t));

    // Init the game
    Game_Init(game);

	// Put in testing characters; will eventually be loaded from a scenario struct
    Game_CreateCharacterAt(game, CHAR_GOOD, true, 5.f, 5.f);
    Game_CreateCharacterAt(game, CHAR_BAD, true, 2.f, 3.f);
    Game_CreateCharacterAt(game, CHAR_UGLY, true, 7.f, 8.f);
    Game_CreateCharacterAt(game, CHAR_ENEMY, false, 2.f, 9.f);
    Board_BuildPlayerControlledCharacterIndex(&game->board);

    // Spike border
    for (u32 i = 0; i < game->grid.grid_w; i++) {
        Game_CreateHazardAt(game, HAZARD_SPIKES, i, 0.f);
        Game_CreateHazardAt(game, HAZARD_SPIKES, i, game->grid.grid_h - 1);
    }

    for (u32 i = 1; i < game->grid.grid_h - 1; i++) {
        Game_CreateHazardAt(game, HAZARD_SPIKES, 0.f, i);
        Game_CreateHazardAt(game, HAZARD_SPIKES, game->grid.grid_w - 1, i);
    }

    Game_CreateHazardAt(game, HAZARD_WATER, 1.f, 1.f);
    
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
