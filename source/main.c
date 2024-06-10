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
#include "audio.h"
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
    CTR_PRINTF("available memory: %ld\n", osGetMemRegionSize(MEMREGION_APPLICATION));

    // Init libs
    romfsInit();
    ndspInit();
    cfguInit();
    gfxInitDefault();
    C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
    C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
    C2D_Prepare();

    // 804Mhz mode
    osSetSpeedupEnable(true);

    // Load audio
    audio_t* audio = malloc(sizeof(audio_t));
    Audio_Init(audio);
    Audio_Play(audio, "romfs:/audio/Deep-Threats.opus");

    // Create screens
    C3D_RenderTarget* top_screen = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);
    C3D_RenderTarget* bottom_screen = C2D_CreateScreenTarget(GFX_BOTTOM, GFX_LEFT);

    // Allocate game data
    game_t* game = malloc(sizeof(game_t));
    memset(game, 0, sizeof(game_t));

    // Init the game
    Game_Init(game);

    // Load scenario
    scenario_t scenario1;
    Scenario_LoadFromJSON(&scenario1, "romfs:/jsons/scenario1.json");
    
    // Run scenario
    Game_LoadScenario(game, &scenario1);
    
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

        if (Input_IsButtonPressed(KEY_B)) {
            //Audio_Play(audio, "romfs:/audio/Vaulted.opus");
        }

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
            C2D_TargetClear(top_screen, C2D_Color32(CLEAR_COLOR_INTS));
            C2D_TargetClear(bottom_screen, C2D_Color32(CLEAR_COLOR_INTS));
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
