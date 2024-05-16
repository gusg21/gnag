#include "game.h"

#include "sprites.h"

void Game_Init(game_t *game) {
    game->sheet = C2D_SpriteSheetLoad("romfs:/gfx/sprites.t3x");
    if (game->sheet == NULL) svcBreak(USERBREAK_PANIC);

    actor_t* actor = &game->actors[0];
    actor->alive = true;
    actor->x = 40;
    C2D_SpriteFromSheet(&actor->sprite, game->sheet, sprites_player_idx);
    C2D_SpriteSetCenter(&actor->sprite, 0.5f, 0.5f);
}

void Game_Destroy(game_t *game) {
    C2D_SpriteSheetFree(game->sheet);
}

void Game_Draw(game_t *game) {
    for (uint32_t i = 0; i < GAME_MAX_ACTORS; i++) {
        actor_t* actor = &game->actors[i];
        C2D_Sprite* sprite = &actor->sprite;
        C2D_SpriteSetPos(sprite, actor->x, actor->y);
        C2D_DrawSprite(sprite);
    }
}
