#ifndef CHARACTER_H
#define CHARACTER_H

#include <3ds.h>
#include <citro2d.h>

#include "characterdata.h"
#include "vec2.h"

#define CHARACTER_MAX_HEALTH 100.f
#define CHARACTER_MOVE_SPEED 5

typedef struct grid_s grid_t;

typedef struct {
    bool initialized;

    character_type_e type;
    vec2_t tile_pos;
    bool is_player_controlled;
    bool moved;
    float health;
    u32 move_speed;

} character_t;

void Character_Init(character_t* character, character_type_e type, bool is_player_controlled, u32 start_x, u32 start_y);
void Character_Draw(character_t* character, C2D_SpriteSheet sheet, grid_t* grid);

vec2_t Character_GetCenterPosition(character_t* character, grid_t* grid);

#endif // CHARACTER_H