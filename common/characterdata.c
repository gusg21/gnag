#include "characterdata.h"

#include "_defs.h"
#include "sprites.h"

#ifdef __cplusplus
extern "C" {
#endif

const char* CharacterData_GetName(character_type_e type) {
    switch (type) {
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

u32 CharacterData_GetSpriteIndexForCharacterType(character_type_e type) {
    switch (type) {
        case CHAR_GOOD:
            return sprites_good_idx;
        case CHAR_BAD:
            return sprites_bad_idx;
        case CHAR_UGLY:
            return sprites_ugly_idx;
        case CHAR_ENEMY:
            return sprites_enemy_idx;

        default:
            return sprites_point_idx;
    }
}

#ifdef __cplusplus
}
#endif