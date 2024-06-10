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

    switch (game->state) {
        case GAME_STATE_NONE:
            break;
        case GAME_STATE_PLAYER_TURN:
            Game_DoPlayerTurn(game);
            break;
        case GAME_STATE_PLAYER_ACTING:
            Game_DoPlayerActing(game);
            break;
        case GAME_STATE_OPPONENT_TURN:
            Game_DoOpponentTurn(game);
            break;
        case GAME_STATE_OPPONENT_ACTING:
            Game_DoOpponentActing(game);
            break;
        case GAME_STATE_SELECTING_TILE:
            Game_DoSelectingTile(game);
            break;

        default:
            break;
    }

    // Move camera to focused target
    game->view_pos = Vec2_Lerp(game->view_pos, game->focus_pos, 0.1f);
}

void Game_Draw(game_t* game) {
    C2D_ViewReset();
    C2D_ViewTranslate(-game->view_pos.x + TOP_SCREEN_WIDTH / 2, -game->view_pos.y + TOP_SCREEN_HEIGHT / 2);

    Grid_Draw(&game->grid);
    Board_Draw(&game->board, game->sheet, &game->grid);

    // Draw tile selection cursor
    if (game->state == GAME_STATE_SELECTING_TILE) {
        C2D_Sprite tile_selected_sprite;
        C2D_SpriteFromSheet(&tile_selected_sprite, game->sheet, sprites_selectedtile_idx);
        C2D_SpriteSetCenter(&tile_selected_sprite, 0.5f, 0.75f);
        C2D_SpriteSetDepth(&tile_selected_sprite, 0.1f);

        if (game->st_type == SELECTING_TILE_MOVE || game->st_type == SELECTING_TILE_SINGLE) {
            vec2_t world_pos = Grid_GridFloatPosToWorldPos(&game->grid, game->selected_tiles[game->current_tile_index]);
            C2D_SpriteSetPos(&tile_selected_sprite, world_pos.x, world_pos.y);
            C2D_DrawSprite(&tile_selected_sprite);
        } else if (game->st_type == SELECTING_TILE_LINE || game->st_type == SELECTING_TILE_CONE) {
            for (u32 i = 0; i <= game->current_tile_index; i++) {
                vec2_t world_pos = Grid_GridFloatPosToWorldPos(&game->grid, game->selected_tiles[i]);
                C2D_SpriteSetPos(&tile_selected_sprite, world_pos.x, world_pos.y);
                C2D_DrawSprite(&tile_selected_sprite);
            }
        }

        if (game->st_type == SELECTING_TILE_MOVE) {
            for (u32 i = 1; i <= game->current_tile_index; i++) {
                vec2_t line_start_pos = Grid_GridFloatPosToWorldPos(&game->grid, game->selected_tiles[i - 1]);
                vec2_t line_end_pos = Grid_GridFloatPosToWorldPos(&game->grid, game->selected_tiles[i]);
                C2D_DrawLine(line_start_pos.x, line_start_pos.y, C2D_Color32f(0.f, 0.f, 1.f, 1.f), line_end_pos.x,
                             line_end_pos.y, C2D_Color32f(0.f, 0.f, 1.f, 1.f), 2.f, 1.f);
            }
        }
    }

    C2D_ViewReset();
}

void Game_LoadScenario(game_t* game, scenario_t* scenario) {
    Grid_Init(&game->grid, game->sheet, scenario->grid_width, scenario->grid_height, GAME_WORLD_CENTER_X,
              GAME_WORLD_CENTER_Y);
    Board_Init(&game->board);

    for (u32 hazard_index = 0; hazard_index < SCENARIO_MAX_HAZARDS; hazard_index++) {
        hazard_data_t* hazard_data = &scenario->hazards[hazard_index];

        if (hazard_data->initialized) {
            Game_CreateHazard(game, hazard_data->type, hazard_data->tile_pos.x, hazard_data->tile_pos.y);
        }
    }

    for (u32 character_index = 0; character_index < SCENARIO_MAX_CHARACTERS; character_index++) {
        character_data_t* character_data = &scenario->characters[character_index];

        if (character_data->initialized) {
            Game_CreateCharacter(game, character_data->type, character_data->is_player_controlled,
                                 character_data->tile_pos.x, character_data->tile_pos.y);
        }
    }

    Board_BuildPlayerControlledCharacterIndex(&game->board);
}

void Game_DoPlayerTurn(game_t* game) {
    if (Input_IsButtonPressed(KEY_B)) {
        if (game->board.next_player_controlled_action_index > 0) {
            Board_UndoLastPlayerControlledCharacterAction(&game->board);
        } else {
            game->board
                .player_controlled_characters_acted_flags[game->board.current_player_controlled_character_index] =
                false;
        }
    } else if (Input_IsButtonPressed(KEY_L)) {
        Board_SelectPreviousPlayerControlledCharacter(&game->board);
    } else if (Input_IsButtonPressed(KEY_R)) {
        Board_SelectNextPlayerControlledCharacter(&game->board);
    }

    game->focus_pos =
        Character_GetCenterPosition(Board_GetCurrentSelectedPlayerControlledCharacter(&game->board), &game->grid);
}

void Game_DoPlayerActing(game_t* game) {
    // All player characters have executed queued actions
    if (!game->board.action_queue_executing) {
        Game_UpdateGameState(game, GAME_STATE_OPPONENT_TURN);
    } else {
        character_action_t* current_action = Board_GetCurrentAction(&game->board);
        switch (current_action->type) {
            case CHARACTER_ACTION_MOVE:
                game->focus_pos =
                    Character_GetCenterPosition(Board_GetCurrentActingCharacter(&game->board), &game->grid);
                break;
            case CHARACTER_ACTION_CREATE_HAZARD:
                game->focus_pos = Grid_GridFloatPosToWorldPos(
                    &game->grid, current_action->tile_selections[current_action->tile_selections_count / 2]);
                break;
            default:
                Character_GetCenterPosition(Board_GetCurrentActingCharacter(&game->board), &game->grid);
                break;
        }
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
    if (game->st_type == SELECTING_TILE_LINE || game->st_type == SELECTING_TILE_CONE) {
        if (Input_IsButtonPressed(KEY_A)) {
            character_action_t action = (character_action_t){.type = CHARACTER_ACTION_CREATE_HAZARD,
                                                             .character = current,
                                                             .hazard_type = game->st_hazard_type,
                                                             .duration = 1.0f,
                                                             .first_frame = true,
                                                             .initialized = true,
                                                             .tile_selections_count = game->current_tile_index + 1};
            for (u32 i = 0; i <= game->current_tile_index; i++) {
                action.tile_selections[i] = game->selected_tiles[i];
            }
            Board_EnqueuePlayerControlledCharacterAction(&game->board, action);
            current->moved = true;
            memset(game->selected_tiles, 0, sizeof(vec2_t) * CHARACTER_ACTION_MAX_TILES_SELECTED);
            game->current_tile_index = 0;
            Game_UpdateGameState(game, GAME_STATE_PLAYER_TURN);
        } else if (Input_IsButtonPressed(KEY_B)) {
            CTR_PRINTF("selection cancelled\n");
            memset(game->selected_tiles, 0, sizeof(vec2_t) * CHARACTER_ACTION_MAX_TILES_SELECTED);
            game->current_tile_index = 0;
            Game_UpdateGameState(game, GAME_STATE_PLAYER_TURN);
        } else {
            vec2_t next_tile_pos = {};

            if (Input_IsButtonPressed(KEY_DUP)) {
                next_tile_pos = (vec2_t){current->tile_pos.x, current->tile_pos.y - 1};
                Game_UpdateSelectedTiles(game, next_tile_pos);
            } else if (Input_IsButtonPressed(KEY_DDOWN)) {
                next_tile_pos = (vec2_t){current->tile_pos.x, current->tile_pos.y + 1};
                Game_UpdateSelectedTiles(game, next_tile_pos);
            } else if (Input_IsButtonPressed(KEY_DLEFT)) {
                next_tile_pos = (vec2_t){current->tile_pos.x - 1, current->tile_pos.y};
                Game_UpdateSelectedTiles(game, next_tile_pos);
            } else if (Input_IsButtonPressed(KEY_DRIGHT)) {
                next_tile_pos = (vec2_t){current->tile_pos.x + 1, current->tile_pos.y};
                Game_UpdateSelectedTiles(game, next_tile_pos);
            }

            game->focus_pos =
                Grid_GridFloatPosToWorldPos(&game->grid, game->selected_tiles[game->current_tile_index / 2]);
        }
    } else if (game->st_type == SELECTING_TILE_MOVE) {
        if (Input_IsButtonPressed(KEY_A)) {
            character_action_t action = (character_action_t){.type = CHARACTER_ACTION_MOVE,
                                                             .character = current,
                                                             .duration = 1.0f,
                                                             .initialized = true,
                                                             .char_pos = current->tile_pos,
                                                             .tile_selections_count = game->current_tile_index};
            for (u32 i = 0; i < game->current_tile_index; i++) {
                action.tile_selections[i] = game->selected_tiles[i + 1];
            }
            Board_EnqueuePlayerControlledCharacterAction(&game->board, action);
            current->moved = true;
            memset(game->selected_tiles, 0, sizeof(vec2_t) * CHARACTER_ACTION_MAX_TILES_SELECTED);
            game->current_tile_index = 0;
            CTR_PRINTF("tile selected\n");
            Game_UpdateGameState(game, GAME_STATE_PLAYER_TURN);
        } else if (Input_IsButtonPressed(KEY_B)) {
            CTR_PRINTF("selection cancelled\n");
            memset(game->selected_tiles, 0, sizeof(vec2_t) * CHARACTER_ACTION_MAX_TILES_SELECTED);
            game->current_tile_index = 0;
            Game_UpdateGameState(game, GAME_STATE_PLAYER_TURN);
        } else {
            vec2_t next_tile_pos = {};

            if (Input_IsButtonPressed(KEY_DUP)) {
                next_tile_pos = (vec2_t){game->selected_tiles[game->current_tile_index].x,
                                         game->selected_tiles[game->current_tile_index].y - 1};
                Game_UpdateSelectedTiles(game, next_tile_pos);
            } else if (Input_IsButtonPressed(KEY_DDOWN)) {
                next_tile_pos = (vec2_t){game->selected_tiles[game->current_tile_index].x,
                                         game->selected_tiles[game->current_tile_index].y + 1};
                Game_UpdateSelectedTiles(game, next_tile_pos);
            } else if (Input_IsButtonPressed(KEY_DLEFT)) {
                next_tile_pos = (vec2_t){game->selected_tiles[game->current_tile_index].x - 1,
                                         game->selected_tiles[game->current_tile_index].y};
                Game_UpdateSelectedTiles(game, next_tile_pos);
            } else if (Input_IsButtonPressed(KEY_DRIGHT)) {
                next_tile_pos = (vec2_t){game->selected_tiles[game->current_tile_index].x + 1,
                                         game->selected_tiles[game->current_tile_index].y};
                Game_UpdateSelectedTiles(game, next_tile_pos);
            }

            game->focus_pos = Grid_GridFloatPosToWorldPos(&game->grid, game->selected_tiles[game->current_tile_index]);
        }
    }
}

character_t* Game_CreateCharacter(game_t* game, character_type_e type, bool is_player_controlled, float tile_x,
                                  float tile_y) {
    character_t* character = Board_NewCharacter(&game->board);
    Character_Init(character, type, is_player_controlled, tile_x, tile_y);

    return character;
}

hazard_t* Game_CreateHazard(game_t* game, hazard_type_e type, float tile_x, float tile_y) {
    hazard_t* hazard = Board_NewHazard(&game->board);
    Hazard_Init(hazard, tile_x, tile_y, type);

    return hazard;
}

bool Game_IsValidTileSelection(game_t* game, vec2_t next_tile_pos) {
    bool in_bounds = next_tile_pos.y >= 0 && next_tile_pos.y <= game->grid.grid_h - 1 && next_tile_pos.x >= 0 &&
                     next_tile_pos.x <= game->grid.grid_w - 1;
    bool in_range = game->current_tile_index < game->tile_select_length;
    bool is_backtrack = next_tile_pos.x == game->selected_tiles[game->current_tile_index - 1].x &&
                        next_tile_pos.y == game->selected_tiles[game->current_tile_index - 1].y;
    if (game->st_type == SELECTING_TILE_MOVE) {
        return in_bounds && (in_range || is_backtrack);
    } else {
        return in_bounds;
    }
}

void Game_UpdateSelectedTiles(game_t* game, vec2_t next_tile_pos) {
    if (Game_IsValidTileSelection(game, next_tile_pos)) {
        switch (game->st_type) {
            case SELECTING_TILE_SINGLE:
                game->selected_tiles[game->current_tile_index] = next_tile_pos;
                return;
            case SELECTING_TILE_MOVE:
                if (next_tile_pos.x == game->selected_tiles[game->current_tile_index - 1].x &&
                    next_tile_pos.y == game->selected_tiles[game->current_tile_index - 1].y) {
                    game->current_tile_index--;
                    game->selected_tiles[game->current_tile_index] = next_tile_pos;
                } else {
                    game->current_tile_index++;
                    game->selected_tiles[game->current_tile_index] = next_tile_pos;
                }
                return;
            case SELECTING_TILE_LINE:
                memset(game->selected_tiles, 0, sizeof(vec2_t) * CHARACTER_ACTION_MAX_TILES_SELECTED);
                game->current_tile_index = 0;
                vec2_t line_dir = {
                    next_tile_pos.x - Board_GetCurrentSelectedPlayerControlledCharacter(&game->board)->tile_pos.x,
                    next_tile_pos.y - Board_GetCurrentSelectedPlayerControlledCharacter(&game->board)->tile_pos.y};
                float half_width = game->tile_select_width / 2;
                for (float i = 0; i < game->tile_select_width; i++) {
                    for (float j = 0; j < game->tile_select_length; j++) {
                        vec2_t next_in_line = {
                            Board_GetCurrentSelectedPlayerControlledCharacter(&game->board)->tile_pos.x +
                                (line_dir.x * (j + 1)) + ((i - half_width) * line_dir.y),
                            Board_GetCurrentSelectedPlayerControlledCharacter(&game->board)->tile_pos.y +
                                (line_dir.y * (j + 1)) + ((i - half_width) * line_dir.x)};
                        if (Game_IsValidTileSelection(game, next_in_line)) {
                            game->selected_tiles[game->current_tile_index] = next_in_line;
                            if (game->current_tile_index + 1 < game->tile_select_length * game->tile_select_width) {
                                game->current_tile_index++;
                            }
                        }
                    }
                }
                if (!(game->current_tile_index + 1 >= game->tile_select_length * game->tile_select_width)) {
                    game->current_tile_index--;
                }
                return;
            case SELECTING_TILE_CONE:
                game->selected_tiles[game->current_tile_index] = next_tile_pos;
                return;

            default:
                // do nothing
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
            for (u32 i = 0; i < game->board.next_player_controlled_action_index; i++) {
                game->board.player_controlled_characters[i]->moved = false;
            }
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
            return;

        default:
            // do nothing
    }
}

void Game_UpdateSelectionType(game_t* game, selecting_tile_type_e type, u32 sel_length, u32 sel_width) {
    if (type == SELECTING_TILE_NONE) return;

    game->st_type = type;
    // These will both only really affect cone and line, and length will affect move
    game->tile_select_length = sel_length;
    game->tile_select_width = sel_width;

    switch (type) {
        case SELECTING_TILE_SINGLE:
            CTR_PRINTF("ST single\n");
            return;
        case SELECTING_TILE_MOVE:
            CTR_PRINTF("ST move\n");
            game->selected_tiles[game->current_tile_index] =
                Board_GetCurrentSelectedPlayerControlledCharacter(&game->board)->tile_pos;
            return;
        case SELECTING_TILE_LINE:
            CTR_PRINTF("ST line\n");
            vec2_t next_tile_pos = {};
            next_tile_pos = (vec2_t){Board_GetCurrentSelectedPlayerControlledCharacter(&game->board)->tile_pos.x + 1,
                                     Board_GetCurrentSelectedPlayerControlledCharacter(&game->board)->tile_pos.y};
            Game_UpdateSelectedTiles(game, next_tile_pos);
            return;
        case SELECTING_TILE_CONE:
            CTR_PRINTF("ST cone\n");
            return;

        default:
            // do nothing
    }
}

void Game_UpdateSelectionHazardType(game_t* game, hazard_type_e type) { game->st_hazard_type = type; }
