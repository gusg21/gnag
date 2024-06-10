#include "hazard.h"

#include "grid.h"
#include "hazardsprites.h"
#include "_defs.h"

static u32 hazard_lut[] = {
 // 0  1  2  3  4  5  6  7  8  9  10 11
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,   // 0
    0, 1, 2, 3, 4, 1, 1, 4, 2, 3, 4, 1,   // 1
    0, 0, 2, 1, 2, 1, 2, 4, 2, 1, 2, 1,   // 2
    0, 0, 0, 3, 4, 3, 1, 4, 1, 3, 4, 3,   // 3
    0, 0, 0, 0, 4, 4, 4, 4, 2, 4, 4, 4,   // 4
    0, 0, 0, 0, 0, 5, 5, 7, 6, 9, 7, 5,   // 5
    0, 0, 0, 0, 0, 0, 6, 7, 6, 5, 6, 0,   // 6
    0, 0, 0, 0, 0, 0, 0, 7, 6, 9, 7, 7,   // 7
    0, 0, 0, 0, 0, 0, 0, 0, 8, 5, 8, 0,   // 8
    0, 0, 0, 0, 0, 0, 0, 0, 0, 9, 9, 9,   // 9
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 10, // 10
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11,  // 11
};

static u32 hazard_lut_width = 12;

void Hazard_Init(hazard_t* hazard, float tile_x, float tile_y, hazard_type_e type) {
    hazard->initialized = true;
    hazard->tile_pos = (vec2_t){tile_x, tile_y};
    hazard->type = type;
}

void Hazard_Draw(hazard_t* hazard, C2D_SpriteSheet sheet, grid_t* grid) {
    C2D_Sprite hazard_sprite;

    switch (hazard->type) {
        case HAZARD_SPIKES: {
            C2D_SpriteFromSheet(&hazard_sprite, sheet, hazardsprites_spikes_idx);
            C2D_SpriteSetCenter(&hazard_sprite, 0.5f, 0.75f);
            break;
        }
        case HAZARD_FLAMING_SPIKES: {
            C2D_SpriteFromSheet(&hazard_sprite, sheet, hazardsprites_flamingspikes_idx);
            C2D_SpriteSetCenter(&hazard_sprite, 0.5f, 0.75f);
            break;
        }
        case HAZARD_WATER: {
            C2D_SpriteFromSheet(&hazard_sprite, sheet, hazardsprites_water_idx);
            C2D_SpriteSetCenter(&hazard_sprite, 0.5f, 0.6f);
            break;
        }
        case HAZARD_FIRE: {
            C2D_SpriteFromSheet(&hazard_sprite, sheet, hazardsprites_fire_idx);
            C2D_SpriteSetCenter(&hazard_sprite, 0.5f, 0.75f);
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
