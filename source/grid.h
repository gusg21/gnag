#ifndef GRID_H
#define GRID_H

#include <citro2d.h>

typedef struct {
    C2D_Sprite empty;
    u32 tile_w, tile_h;
    u32 grid_w, grid_h;
    s32 center_x, center_y;

    C2D_Sprite debug_point_sprite;
} grid_t;

void Grid_Init(grid_t* grid, C2D_SpriteSheet sheet, size_t empty_index, u32 grid_w, u32 grid_h, s32 center_x, s32 center_y);
void Grid_Draw(grid_t* grid);

#endif // GRID_H