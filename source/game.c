#include "game.h"

#include "_defs.h"
#include "sprites.h"
#include "input.h"

void Game_Init(game_t *game) {
    game->sheet = C2D_SpriteSheetLoad("romfs:/gfx/sprites.t3x");
    if (game->sheet == NULL) svcBreak(USERBREAK_PANIC);

    Grid_Init(&game->grid, game->sheet, sprites_emptytile_idx, 10, 10, GAME_WORLD_CENTER_X, GAME_WORLD_CENTER_Y);

    game->view_x = GAME_WORLD_CENTER_X;
    game->view_y = GAME_WORLD_CENTER_Y;
}

void Game_Destroy(game_t *game) {
    C2D_SpriteSheetFree(game->sheet);
}

void Game_Update(game_t *game, float delta_secs) {
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

void Game_Draw(game_t *game) {
    C2D_ViewReset();
    C2D_ViewTranslate(-game->view_x + SCREEN_WIDTH / 2, -game->view_y + SCREEN_HEIGHT / 2);

    Grid_Draw(&game->grid);

    for (uint32_t i = 0; i < GAME_MAX_ACTORS; i++) {
        actor_t* actor = &game->actors[i];
        C2D_Sprite* sprite = &actor->sprite;
        C2D_SpriteSetPos(sprite, actor->x, actor->y);
        C2D_DrawSprite(sprite);
    }

    C2D_ViewReset();
}
