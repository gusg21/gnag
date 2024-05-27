#include "hazard.h"

#include "grid.h"
#include "sprites.h"

void Hazard_Init(hazard_t* hazard, float tile_x, float tile_y, hazard_type_e type) {
    hazard->initialized = true;
    hazard->tile_pos = (vec2_t) {tile_x, tile_y};
    hazard->type = type;
}

void Hazard_Draw(hazard_t* hazard, C2D_SpriteSheet sheet, grid_t* grid) {
    C2D_Sprite hazard_sprite;

    switch (hazard->type)
    {
    case HAZARD_SPIKES:
    {
        C2D_SpriteFromSheet(&hazard_sprite, sheet, sprites_spikes_idx);
        C2D_SpriteSetCenter(&hazard_sprite, 0.5f, 0.75f);
        break;
    }
    case HAZARD_WATER:
    {
        C2D_SpriteFromSheet(&hazard_sprite, sheet, sprites_watertile_idx);
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