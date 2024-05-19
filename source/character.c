#include "character.h"

#include "sprites.h"

static u32 S_GetSpriteIndexForCharacterType(character_type_e type) {
    switch (type) {
        case CHAR_GOOD:
            return sprites_good_idx;
        case CHAR_BAD:
            return sprites_bad_idx;

        default:
            return sprites_point_idx;
    }
}

void Character_Init(character_t* character, C2D_SpriteSheet sheet, character_type_e type, bool is_player_controlled,
                    u32 start_x, u32 start_y) {
    character->initialized = true;
    character->pos.x = start_x;
    character->pos.y = start_y;
    character->type = type;
    character->is_player_controlled = is_player_controlled;
    character->health = CHARACTER_MAX_HEALTH;

    C2D_SpriteFromSheet(&character->sprite, sheet, S_GetSpriteIndexForCharacterType(type));
    C2D_SpriteSetCenter(&character->sprite, 0.5f, 1.0f);  // Bottom center
    C2D_SpriteSetDepth(&character->sprite, 1.f);
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

void Character_Draw(character_t* character) {
    C2D_SpriteSetPos(&character->sprite, character->pos.x, character->pos.y);
    C2D_DrawSprite(&character->sprite);
}
