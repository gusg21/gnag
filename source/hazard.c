#include "hazard.h"

#include "grid.h"
#include "sprites.h"
#include "_defs.h"

static u32 hazard_lut[] = {
    0, 0, 0,
    -1, 1, 1,
    -1, -1, 2,
};
static u32 hazard_lut_width = 3;

void Hazard_Init(hazard_t* hazard, float tile_x, float tile_y, hazard_type_e type) {
    hazard->initialized = true;
    hazard->tile_pos = (vec2_t){tile_x, tile_y};
    hazard->type = type;
}

void Hazard_Draw(hazard_t* hazard, C2D_SpriteSheet sheet, grid_t* grid) {
    C2D_Sprite hazard_sprite;
    u32 hazard_sprite_index = HazardData_GetSpriteIndexForHazardType(hazard->type);
    C2D_SpriteFromSheet(&hazard_sprite, sheet, hazard_sprite_index);

    // TODO)) @gusg21 move the pivots to a vec2_t HazardData_GetPivotForHazardType() or smth
    switch (hazard->type) {
        case HAZARD_SPIKES: {
            C2D_SpriteSetCenter(&hazard_sprite, 0.5f, 0.75f);
            break;
        }
        case HAZARD_WATER: {
            C2D_SpriteSetCenter(&hazard_sprite, 0.5f, 0.6f);
            break;
        }

        case HAZARD_NONE:
        default:
            break;
    }

    vec2_t world_pos = Grid_GridFloatPosToWorldPos(grid, hazard->tile_pos);
    C2D_SpriteSetPos(&hazard_sprite, world_pos.x, world_pos.y);
    C2D_DrawSprite(&hazard_sprite);
}

hazard_type_e Hazard_Combine(hazard_type_e hazard_type_1, hazard_type_e hazard_type_2) {
    u32 min = MIN(hazard_type_1, hazard_type_2);
    u32 max = MAX(hazard_type_1, hazard_type_2);
    return hazard_lut[max + (min * hazard_lut_width)];
}
