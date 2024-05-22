#include "game.h"

#include "_defs.h"
#include "ai.h"
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

    // PLAYERS ACTING
    if (game->state == GAME_STATE_PLAYER_ACTING) {
        // All player characters have executed queued actions
        if (!game->board.action_queue_executing) {
            Game_UpdateGameState(game, GAME_STATE_OPPONENT_TURN);
        } else {
            game->focus_pos = Character_GetCenterPosition(Board_GetCurrentActingCharacter(&game->board), &game->grid);
        }
    
    // PLAYERS TURN
    } else if (game->state == GAME_STATE_PLAYER_TURN) {
        game->focus_pos = Character_GetCenterPosition(Board_GetCurrentSelectedPlayerControlledCharacter(&game->board), &game->grid);

    // PLAYER SELECTING TILE
    } else if (game->state == GAME_STATE_SELECTING_TILE) {
        // Return selection on A
        if (Input_IsButtonPressed(KEY_A)) {
            character_t* current = Board_GetCurrentSelectedPlayerControlledCharacter(&game->board);
            character_action_t action = (character_action_t){.type = CHARACTER_ACTION_MOVE,
                                                             .character = current,
                                                             .duration = 1.0f,
                                                             .initialized = true,
                                                             .move_source = current->tile_pos,
                                                             .move_destination_count = 1};
            action.move_destinations[0] = game->selected_tile_pos;
            Board_EnqueuePlayerControlledCharacterAction(&game->board, action);
            CTR_PRINTF("tile selected\n");
            Game_UpdateGameState(game, GAME_STATE_PLAYER_TURN);
        }
        // Return without selecting on B
        else if (Input_IsButtonPressed(KEY_B)) {
            CTR_PRINTF("selection cancelled\n");
            Game_UpdateGameState(game, GAME_STATE_PLAYER_TURN);
        }
        else {
            // 4 input directions to select tile
            if (Input_IsButtonPressed(KEY_DUP)) {
                game->selected_tile_pos = (vec2_t){game->selected_tile_pos.x + 1, game->selected_tile_pos.y};
                CTR_PRINTF("up right\n");
            }
            else if (Input_IsButtonPressed(KEY_DLEFT)) {
                game->selected_tile_pos = (vec2_t){game->selected_tile_pos.x, game->selected_tile_pos.y - 1};
                CTR_PRINTF("up left\n");
            }
            else if (Input_IsButtonPressed(KEY_DRIGHT)) {
                game->selected_tile_pos = (vec2_t){game->selected_tile_pos.x, game->selected_tile_pos.y + 1};
                CTR_PRINTF("down right\n");
            }
            else if (Input_IsButtonPressed(KEY_DDOWN)) {
                game->selected_tile_pos = (vec2_t){game->selected_tile_pos.x - 1, game->selected_tile_pos.y};
                CTR_PRINTF("down left\n");
            }
            // Focus is selected tile
            game->focus_pos = Grid_GridFloatPosToWorldPos(&game->grid, game->selected_tile_pos);
        }

    // OPPONENTS TURN
    } else if (game->state == GAME_STATE_OPPONENT_TURN) {
        // Do the opponent AI
        AI_EnqueueAIActions(&game->ai, &game->board);
        Board_ExecuteQueue(&game->board);

        // Immediately change to acting state
        Game_UpdateGameState(game, GAME_STATE_OPPONENT_ACTING);

    // OPPONENTS ACTING
    } else if (game->state == GAME_STATE_OPPONENT_ACTING) {
        // Wait until all actions complete
        if (!game->board.action_queue_executing) {
            // Return to player turn
            Game_UpdateGameState(game, GAME_STATE_PLAYER_TURN);
        } else {
            game->focus_pos = Character_GetCenterPosition(Board_GetCurrentActingCharacter(&game->board), &game->grid);
        }
    }

    // Move camera to focused target
    game->view_pos = Vec2_Lerp(game->view_pos, game->focus_pos, 0.1f);
}

void Game_Draw(game_t* game) {
    C2D_ViewReset();
    C2D_ViewTranslate(-game->view_pos.x + TOP_SCREEN_WIDTH / 2, -game->view_pos.y + TOP_SCREEN_HEIGHT / 2);

    Grid_Draw(&game->grid);
    Board_Draw(&game->board, &game->grid);

    if (game->state == GAME_STATE_SELECTING_TILE) {
        C2D_Sprite tile_selected_sprite;
        C2D_SpriteFromSheet(&tile_selected_sprite, game->sheet, sprites_selectedtile_idx);
        vec2_t world_pos = Grid_GridFloatPosToWorldPos(&game->grid, game->selected_tile_pos);
        C2D_SpriteSetPos(&tile_selected_sprite, world_pos.x, world_pos.y);
        C2D_SpriteSetCenter(&tile_selected_sprite, 0.5f, 0.75f);
        C2D_SpriteSetDepth(&tile_selected_sprite, 0.5f);
        C2D_DrawSprite(&tile_selected_sprite);
    }


    C2D_ViewReset();
}

character_t* Game_CreateCharacterAt(game_t* game, character_type_e type, bool is_player_controlled, float tile_x,
                                    float tile_y) {
    character_t* character = Board_NewCharacter(&game->board);
    Character_Init(character, game->sheet, type, is_player_controlled, tile_x, tile_y);

    return character;
}

void Game_UpdateGameState(game_t* game, game_state_e state) {
    if (state == GAME_STATE_NONE) return;

    game->state = state;

    switch (state) {
        case GAME_STATE_PAUSED:
            CTR_PRINTF("STATE paused\n");
            return;
        case GAME_STATE_PLAYER_TURN:
            CTR_PRINTF("STATE player turn\n");
            return;
        case GAME_STATE_PLAYER_ACTING:
            CTR_PRINTF("STATE player acting\n");
            return;
        case GAME_STATE_OPPONENT_TURN:  // todo)) actually needed? enemies would most likely act instantly with no
                                        // "thinking"
            CTR_PRINTF("STATE opponent turn\n");
            return;
        case GAME_STATE_OPPONENT_ACTING:
            CTR_PRINTF("STATE opponent acting\n");
            return;
        case GAME_STATE_SELECTING_TILE:
            CTR_PRINTF("STATE player selecting tile\n");
            game->selected_tile_pos = Board_GetCurrentSelectedPlayerControlledCharacter(&game->board)->tile_pos;
            return;

        default:
            // do nothing
    }
}
