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

    if (game->state == GAME_STATE_PLAYER_TURN) {
        Game_DoPlayerTurn(game);
    } else if (game->state == GAME_STATE_PLAYER_ACTING) {
        Game_DoPlayerActing(game);
    } else if (game->state == GAME_STATE_OPPONENT_TURN) {
        Game_DoOpponentTurn(game);
    } else if (game->state == GAME_STATE_OPPONENT_ACTING) {
        Game_DoOpponentActing(game);
    } else if (game->state == GAME_STATE_SELECTING_TILE) {
        Game_DoSelectingTile(game);
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
        vec2_t world_pos = Grid_GridFloatPosToWorldPos(&game->grid, game->selected_tiles[game->current_tile_index]);
        C2D_SpriteSetPos(&tile_selected_sprite, world_pos.x, world_pos.y);
        C2D_SpriteSetCenter(&tile_selected_sprite, 0.5f, 0.75f);
        C2D_SpriteSetDepth(&tile_selected_sprite, 0.5f);
        C2D_DrawSprite(&tile_selected_sprite);

        for (u32 i = 1; i <= game->current_tile_index; i++) {
            vec2_t line_start_pos = Grid_GridFloatPosToWorldPos(&game->grid, game->selected_tiles[i - 1]);
            vec2_t line_end_pos = Grid_GridFloatPosToWorldPos(&game->grid, game->selected_tiles[i]);
            C2D_DrawLine(line_start_pos.x, line_start_pos.y, C2D_Color32f(0.f, 0.f, 1.f, 1.f),
                         line_end_pos.x, line_end_pos.y, C2D_Color32f(0.f, 0.f, 1.f, 1.f),
                         2.f, 1.f);
        }
    }

    C2D_ViewReset();
}

void Game_DoPlayerTurn(game_t* game) {
    if (Input_IsButtonPressed(KEY_B)) {
        Board_UndoLastPlayerControlledCharacterAction(&game->board);
    }
    game->focus_pos =
        Character_GetCenterPosition(Board_GetCurrentSelectedPlayerControlledCharacter(&game->board), &game->grid);
}

void Game_DoPlayerActing(game_t* game) {
    // All player characters have executed queued actions
    if (!game->board.action_queue_executing) {
        Game_UpdateGameState(game, GAME_STATE_OPPONENT_TURN);
    } else {
        game->focus_pos = Character_GetCenterPosition(Board_GetCurrentActingCharacter(&game->board), &game->grid);
    }
}

void Game_DoOpponentTurn(game_t* game) {
    // Do the opponent AI
    AI_EnqueueAIActions(&game->ai, &game->board);
    Board_ExecuteQueue(&game->board);

    // Immediately change to acting state
    Game_UpdateGameState(game, GAME_STATE_OPPONENT_ACTING);
}

void Game_DoOpponentActing(game_t* game) {
    // Wait until all actions complete
    if (!game->board.action_queue_executing) {
        // Return to player turn
        Game_UpdateGameState(game, GAME_STATE_PLAYER_TURN);
    } else {
        game->focus_pos = Character_GetCenterPosition(Board_GetCurrentActingCharacter(&game->board), &game->grid);
    }
}

void Game_DoSelectingTile(game_t* game) {
    character_t* current = Board_GetCurrentSelectedPlayerControlledCharacter(&game->board);

    if (Input_IsButtonPressed(KEY_A)) {
        character_action_t action = (character_action_t){.type = CHARACTER_ACTION_MOVE,
                                                         .character = current,
                                                         .duration = 1.0f,
                                                         .initialized = true,
                                                         .move_source = current->tile_pos,
                                                         .move_destination_count = game->current_tile_index};
        for (u32 i = 0; i < game->current_tile_index; i++) {
            action.move_destinations[i] = game->selected_tiles[i + 1];
        }
        Board_EnqueuePlayerControlledCharacterAction(&game->board, action);
        current->moved = true;
        CTR_PRINTF("tile selected\n");
        memset(game->selected_tiles, 0, sizeof(vec2_t));
        game->current_tile_index = 0;
        Game_UpdateGameState(game, GAME_STATE_PLAYER_TURN);
    } else if (Input_IsButtonPressed(KEY_B)) {
        CTR_PRINTF("selection cancelled\n");
        memset(game->selected_tiles, 0, sizeof(vec2_t));
        game->current_tile_index = 0;
        Game_UpdateGameState(game, GAME_STATE_PLAYER_TURN);
    } else {
        vec2_t next_tile_pos = {};

        if (Input_IsButtonPressed(KEY_DUP)) {
            next_tile_pos = (vec2_t){game->selected_tiles[game->current_tile_index].x, game->selected_tiles[game->current_tile_index].y - 1};
            Game_UpdateSelectedTiles(game, next_tile_pos);
        } else if (Input_IsButtonPressed(KEY_DDOWN)) {
            next_tile_pos = (vec2_t){game->selected_tiles[game->current_tile_index].x, game->selected_tiles[game->current_tile_index].y + 1};
            Game_UpdateSelectedTiles(game, next_tile_pos);
        } else if (Input_IsButtonPressed(KEY_DLEFT)) {
            next_tile_pos = (vec2_t){game->selected_tiles[game->current_tile_index].x - 1, game->selected_tiles[game->current_tile_index].y};
            Game_UpdateSelectedTiles(game, next_tile_pos);
        } else if (Input_IsButtonPressed(KEY_DRIGHT)) {
            next_tile_pos = (vec2_t){game->selected_tiles[game->current_tile_index].x + 1, game->selected_tiles[game->current_tile_index].y};
            Game_UpdateSelectedTiles(game, next_tile_pos);
        }

        game->focus_pos = Grid_GridFloatPosToWorldPos(&game->grid, game->selected_tiles[game->current_tile_index]);
    }
}

character_t* Game_CreateCharacterAt(game_t* game, character_type_e type, bool is_player_controlled, float tile_x,
                                    float tile_y) {
    character_t* character = Board_NewCharacter(&game->board);
    Character_Init(character, game->sheet, type, is_player_controlled, tile_x, tile_y);

    return character;
}

bool Game_IsValidTileSelection(game_t* game, vec2_t next_tile_pos) {
    return
        // Position in grid
        (next_tile_pos.y >= 0 && next_tile_pos.y <= game->grid.grid_h - 1 &&
        next_tile_pos.x >= 0 && next_tile_pos.x <= game->grid.grid_w - 1 &&
        // Move speed & backtracking
        (!(game->current_tile_index >= Board_GetCurrentSelectedPlayerControlledCharacter(&game->board)->move_speed) ||
        (next_tile_pos.x == game->selected_tiles[game->current_tile_index - 1].x && 
        next_tile_pos.y == game->selected_tiles[game->current_tile_index - 1].y)));
}

void Game_UpdateSelectedTiles(game_t* game, vec2_t next_tile_pos) {
    if (Game_IsValidTileSelection(game, next_tile_pos)) {
        if (next_tile_pos.x == game->selected_tiles[game->current_tile_index - 1].x && 
        next_tile_pos.y == game->selected_tiles[game->current_tile_index - 1].y) {
            game->current_tile_index--;
            game->selected_tiles[game->current_tile_index] = next_tile_pos;
        } else {
            game->current_tile_index++;
            game->selected_tiles[game->current_tile_index] = next_tile_pos;
        }
    }
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
            game->selected_tiles[game->current_tile_index] = Board_GetCurrentSelectedPlayerControlledCharacter(&game->board)->tile_pos;
            return;

        default:
            // do nothing
    }
}
