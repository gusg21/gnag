#include "character.h"

#include "sprites.h"
#include "board.h"

static u32 S_GetSpriteIndexForCharacterType(character_type_e type) {
    switch (type) {
        case CHAR_GOOD:
            return sprites_good_idx;
        case CHAR_BAD:
            return sprites_bad_idx;
        case CHAR_UGLY:
            return sprites_ugly_idx;

        default:
            return sprites_point_idx;
    }
}

void Character_Init(character_t* character, C2D_SpriteSheet sheet, character_type_e type, bool is_player_controlled,
                    u32 start_x, u32 start_y) {
    character->initialized = true;
    character->tile_pos.x = start_x;
    character->tile_pos.y = start_y;
    character->type = type;
    character->is_player_controlled = is_player_controlled;
    character->health = CHARACTER_MAX_HEALTH;

    C2D_SpriteFromSheet(&character->sprite, sheet, S_GetSpriteIndexForCharacterType(type));
    C2D_SpriteSetCenter(&character->sprite, 0.5f, 1.0f);  // Bottom center
    C2D_SpriteSetDepth(&character->sprite, 1.f);
}

void Character_Draw(character_t* character, grid_t* grid) {
    vec2_t world_pos = Grid_GridFloatPosToWorldPos(grid, character->tile_pos);
    world_pos.y += 10.f;
    C2D_SpriteSetPos(&character->sprite, world_pos.x, world_pos.y);
    C2D_DrawSprite(&character->sprite);
}

const char* Character_GetName(character_t* character) {
    switch (character->type) {
        case CHAR_BAD:
            return "Bad";
        case CHAR_ENEMY:
            return "Enemy";
        case CHAR_GOOD:
            return "Good";
        case CHAR_NONE:
            return "Ted";
        case CHAR_UGLY:
            return "Ugly";

        default:
            return "Ted";
    }
}

vec2_t Character_GetCenterPosition(character_t* character, grid_t* grid) {
    // This function might vary based on type for different sized character sprites. for now this should work; change if
    // needed ;)
    vec2_t world_pos = Grid_GridFloatPosToWorldPos(grid, character->tile_pos);
    return (vec2_t){.x = world_pos.x, .y = world_pos.y - 48.f};
}