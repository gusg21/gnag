#include "character.h"

#include "board.h"
#include "sprites.h"

void Character_Init(character_t* character, character_type_e type, bool is_player_controlled,
                    u32 start_x, u32 start_y) {
    character->initialized = true;
    character->tile_pos.x = start_x;
    character->tile_pos.y = start_y;
    character->type = type;
    character->is_player_controlled = is_player_controlled;
    character->moved = false;
    character->health = CHARACTER_MAX_HEALTH;
    character->move_speed = CHARACTER_MOVE_SPEED;
}

void Character_Draw(character_t* character, C2D_SpriteSheet sheet, grid_t* grid) {
    vec2_t world_pos = Grid_GridFloatPosToWorldPos(grid, character->tile_pos);
    world_pos.y += 10.f;

    C2D_Sprite character_sprite;
    C2D_SpriteFromSheet(&character_sprite, sheet, CharacterData_GetSpriteIndexForCharacterType(character->type));
    C2D_SpriteSetCenter(&character_sprite, 0.5f, 1.0f);  // Bottom center
    C2D_SpriteSetDepth(&character_sprite, 0.f);
    C2D_SpriteSetPos(&character_sprite, world_pos.x, world_pos.y);
    C2D_DrawSprite(&character_sprite);
}

vec2_t Character_GetCenterPosition(character_t* character, grid_t* grid) {
    // This function might vary based on type for different sized character sprites. for now this should work; change if
    // needed ;)
    vec2_t world_pos = Grid_GridFloatPosToWorldPos(grid, character->tile_pos);
    return (vec2_t){.x = world_pos.x, .y = world_pos.y - 48.f};
}