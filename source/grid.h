#ifndef GRID_H
#define GRID_H

#include <citro2d.h>

#include "vec2.h"

#define GRID_MAX_TILE_DATAS 256

typedef struct {
    bool is_spikes;
} grid_tile_t;

typedef struct {
    C2D_Sprite empty;
    C2D_Sprite spikes;
    u32 tile_w, tile_h;
    u32 grid_w, grid_h;
    s32 center_x, center_y;

    grid_tile_t tile_data[GRID_MAX_TILE_DATAS];

    C2D_Sprite debug_point_sprite;
} grid_t;

void Grid_Init(grid_t* grid, C2D_SpriteSheet sheet, size_t empty_index, size_t spikes_index, u32 grid_w, u32 grid_h, s32 center_x, s32 center_y);
void Grid_Draw(grid_t* grid);
C2D_Sprite* Grid_GetSpriteForTile(grid_t* grid, grid_tile_t* tile);
grid_tile_t* Grid_GetTileAt(grid_t* grid, u32 tile_x, u32 tile_y);
vec2_t Grid_GridPosToWorldPos(grid_t* grid, vec2i_t grid_pos);
float Grid_GetIsoDepthForTilePos(grid_t* grid, u32 tile_x, u32 tile_y);

#endif // GRID_H