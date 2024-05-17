#include "game.h"

#include "_defs.h"
#include "input.h"
#include "panic.h"
#include "sprites.h"

void Game_Init(game_t* game) {
    game->sheet = C2D_SpriteSheetLoad("romfs:/gfx/sprites.t3x");
    if (game->sheet == NULL) {
        Panic_Panic();
        printf("Failed to load sprite sheet from fs");
    }

    Board_Init(&game->board);
    Grid_Init(&game->grid, game->sheet, 10, 10, GAME_WORLD_CENTER_X, GAME_WORLD_CENTER_Y);

    game->view_x = GAME_WORLD_CENTER_X;
    game->view_y = GAME_WORLD_CENTER_Y;
}

void Game_Destroy(game_t* game) { C2D_SpriteSheetFree(game->sheet); }

void Game_Update(game_t* game, float delta_secs) {
    float cam_speed = 100.f;

    if (Input_IsButtonDown(KEY_CPAD_RIGHT)) {
        game->view_x += delta_secs * cam_speed;
    }
    if (Input_IsButtonDown(KEY_CPAD_LEFT)) {
        game->view_x -= delta_secs * cam_speed;
    }
    if (Input_IsButtonDown(KEY_CPAD_UP)) {
        game->view_y -= delta_secs * cam_speed;
    }
    if (Input_IsButtonDown(KEY_CPAD_DOWN)) {
        game->view_y += delta_secs * cam_speed;
    }
}

void Game_Draw(game_t* game) {
    C2D_ViewReset();
    C2D_ViewTranslate(-game->view_x + SCREEN_WIDTH / 2, -game->view_y + SCREEN_HEIGHT / 2);

    Grid_Draw(&game->grid);
    Board_Draw(&game->board, &game->grid);

    C2D_ViewReset();
}

character_t* Game_CreateCharacterAt(game_t* game, character_type_e type, u32 tile_x, u32 tile_y) {
    vec2_t world_pos = Grid_GridPosToWorldPos(&game->grid, (vec2i_t){.x = tile_x, .y = tile_y});
    world_pos.y += 10;  // centers the sprites better

    character_t* character = Board_NewCharacter(&game->board);
    Character_Init(character, game->sheet, type, world_pos.x, world_pos.y);

    return character;
}
