#ifndef CHARACTER_H
#define CHARACTER_H

#include <3ds.h>
#include <citro2d.h>

typedef enum {
    CHAR_NONE,
    CHAR_GOOD,
    CHAR_BAD,
    CHAR_UGLY,
    CHAR_ENEMY
} character_type_e;

typedef struct {
    bool valid;

    character_type_e type;

    u32 pos_x, pos_y;

    C2D_Sprite sprite;
} character_t;

void Character_Init(character_t* character, C2D_SpriteSheet sheet, character_type_e type, u32 start_x, u32 start_y);
void Character_Draw(character_t* character);

#endif // CHARACTER_H