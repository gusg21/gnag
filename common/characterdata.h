#ifndef CHARACTERDATA_H
#define CHARACTERDATA_H

#include "_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    CHAR_NONE,
    CHAR_GOOD,
    CHAR_BAD,
    CHAR_UGLY,
    CHAR_ENEMY,
    CHAR_COUNT
} character_type_e;


const char* CharacterData_GetName(character_type_e type);
u32 CharacterData_GetSpriteIndexForCharacterType(character_type_e type);

#ifdef __cplusplus
}
#endif

#endif // CHARACTERDATA_H