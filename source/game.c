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

    game->view_pos.x = GAME_WORLD_CENTER_X;
    game->view_pos.y = GAME_WORLD_CENTER_Y;

    Game_UpdateGameState(game, GAME_STATE_PLAYER_TURN);
}

void Game_Destroy(game_t* game) { C2D_SpriteSheetFree(game->sheet); }

void Game_Update(game_t* game, float delta_secs) {
    Board_Update(&game->board, delta_secs);
    
    if (game->state == GAME_STATE_PLAYER_ACTING) {
        //game->focused_character = Board_GetCurrentActingCharacter(&game->board);

        // All player characters have executed queued actions
        if (!game->board.action_queue_executing) {
            Game_UpdateGameState(game, GAME_STATE_PLAYER_TURN);
        }
    } else if (game->state == GAME_STATE_PLAYER_TURN) {
        game->focused_character = Board_GetCurrentSelectedPlayerControlledCharacter(&game->board);

    } else if (game->state == GAME_STATE_SELECTING_TILE) {
        // Return selection on A
        if (Input_IsButtonPressed(KEY_A)) {
            character_t* current = Board_GetCurrentSelectedPlayerControlledCharacter(&game->board);
            Board_EnqueuePlayerControlledCharacterAction(&game->board,
                        (character_action_t){.character = current,
                                             .duration = 0.5f,
                                             .initialized = true,
                                             .type = ACTION_MOVE,
                                             .move_source = current->pos,
                                             .move_destination = Vec2_Add(current->pos, (vec2_t){32.f, -16.f})});
            CTR_PRINTF("tile selected\n");
            Game_UpdateGameState(game, GAME_STATE_PLAYER_TURN);
        }
        // Return without selecting on B
        else if (Input_IsButtonPressed(KEY_B)) {
            CTR_PRINTF("selection cancelled\n");
            Game_UpdateGameState(game, GAME_STATE_PLAYER_TURN);
        }
    }

    vec2_t character_pos = Character_GetCenterPosition(game->focused_character);
    game->view_pos = Vec2_Lerp(game->view_pos, character_pos, 0.1f);
}

void Game_Draw(game_t* game) {
    C2D_ViewReset();
    C2D_ViewTranslate(-game->view_pos.x + TOP_SCREEN_WIDTH / 2, -game->view_pos.y + TOP_SCREEN_HEIGHT / 2);

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
            CTR_PRINTF("STATE paused\n");
            return;
        case GAME_STATE_PLAYER_TURN:
            CTR_PRINTF("STATE player turn\n");
            return;
        case GAME_STATE_PLAYER_ACTING:
            CTR_PRINTF("STATE player acting\n");
            return;
        case GAME_STATE_OPPONENT_TURN: // todo)) actually needed? enemies would most likely act instantly with no "thinking"
            CTR_PRINTF("STATE opponent turn\n");
            return;
        case GAME_STATE_OPPONENT_ACTING:
            CTR_PRINTF("STATE opponent acting\n");
            return;
        case GAME_STATE_SELECTING_TILE:
            CTR_PRINTF("STATE player selecting tile\n");
            return;

        default:
        //do nothing
    }
}
