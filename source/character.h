#ifndef CHARACTER_H
#define CHARACTER_H

#include <3ds.h>
#include <citro2d.h>

#include "vec2.h"

typedef enum {
    CHAR_NONE,
    CHAR_GOOD,
    CHAR_BAD,
    CHAR_UGLY,
    CHAR_ENEMY
} character_type_e;

typedef struct {
    bool initialized;

    character_type_e type;
    vec2_t pos;
    C2D_Sprite sprite;
    bool is_player_controlled;
} character_t;

void Character_Init(character_t* character, C2D_SpriteSheet sheet, character_type_e type, bool is_player_controlled, u32 start_x, u32 start_y);
void Character_Draw(character_t* character);

#endif // CHARACTER_H