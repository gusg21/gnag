#include "game.h"

#include "_defs.h"
#include "input.h"
#include "panic.h"
#include "sprites.h"

void Game_Init(game_t* game) {
    game->sheet = C2D_SpriteSheetLoad("romfs:/gfx/sprites.t3x");
    if (game->sheet == NULL) {
        Panic_Panic();
        CTR_PRINTF("Failed to load sprite sheet from fs\n");
    }

    Board_Init(&game->board);
    Grid_Init(&game->grid, game->sheet, 10, 10, GAME_WORLD_CENTER_X, GAME_WORLD_CENTER_Y);

    game->view.x = GAME_WORLD_CENTER_X;
    game->view.y = GAME_WORLD_CENTER_Y;

    Game_UpdateGameState(game, GAME_STATE_PLAYER_TURN);
}

void Game_Destroy(game_t* game) { C2D_SpriteSheetFree(game->sheet); }

void Game_Update(game_t* game, float delta_secs) {
    Board_Update(&game->board, delta_secs);

    // If all player characters have acted, set next game state
    if (game->state == GAME_STATE_PLAYER_ACTING && !game->board.action_queue_executing)
    {
        Game_UpdateGameState(game, GAME_STATE_PLAYER_TURN);
    }

    float cam_speed = 100.f;

    if (Input_IsButtonDown(KEY_CPAD_RIGHT)) {
        game->view.x += delta_secs * cam_speed;
    }
    if (Input_IsButtonDown(KEY_CPAD_LEFT)) {
        game->view.x -= delta_secs * cam_speed;
    }
    if (Input_IsButtonDown(KEY_CPAD_UP)) {
        game->view.y -= delta_secs * cam_speed;
    }
    if (Input_IsButtonDown(KEY_CPAD_DOWN)) {
        game->view.y += delta_secs * cam_speed;
    }
}

void Game_Draw(game_t* game) {
    C2D_ViewReset();
    C2D_ViewTranslate(-game->view.x + TOP_SCREEN_WIDTH / 2, -game->view.y + TOP_SCREEN_HEIGHT / 2);

    Grid_Draw(&game->grid);
    Board_Draw(&game->board, &game->grid);

    C2D_ViewReset();
}

character_t* Game_CreateCharacterAt(game_t* game, character_type_e type, bool is_player_controlled, u32 tile_x, u32 tile_y) {
    vec2_t world_pos = Grid_GridPosToWorldPos(&game->grid, (vec2i_t){.x = tile_x, .y = tile_y});
    world_pos.y += 10;  // centers the sprites better

    character_t* character = Board_NewCharacter(&game->board);
    Character_Init(character, game->sheet, type, is_player_controlled, world_pos.x, world_pos.y);

    return character;
}

void Game_UpdateGameState(game_t* game, game_state_e state) {
    if (state == GAME_STATE_NONE) return;
    
    game->state = state;

    switch (state)
    {
        case GAME_STATE_PAUSED:
            // Do nothing
        case GAME_STATE_PLAYER_TURN:
            CTR_PRINTF("player turn");
        case GAME_STATE_PLAYER_ACTING:
            CTR_PRINTF("player acting");
        case GAME_STATE_OPPONENT_TURN:
            // Center camera on active player
        case GAME_STATE_OPPONENT_ACTING:
            // Center camera on active player
        case GAME_STATE_SELECTING_TILE:
            // Center camera on active player

        default:
        //do nothing
    }
}
