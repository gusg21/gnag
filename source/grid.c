#include "grid.h"

#include "sprites.h"

void Grid_Init(grid_t* grid, C2D_SpriteSheet sheet, size_t empty_index, u32 grid_w, u32 grid_h, s32 center_x, s32 center_y) {
    C2D_SpriteFromSheet(&grid->empty, sheet, empty_index);

    grid->tile_w = (u32)grid->empty.image.subtex->width;
    grid->tile_h = (u32)grid->empty.image.subtex->height;

    grid->grid_w = grid_w;
    grid->grid_h = grid_h;

    grid->center_x = center_x;
    grid->center_y = center_y;

    C2D_SpriteFromSheet(&grid->debug_point_sprite, sheet, sprites_point_idx);
    C2D_SpriteSetCenter(&grid->debug_point_sprite, 0.5f, 0.5f);
    C2D_SpriteSetPos(&grid->debug_point_sprite, grid->center_x, grid->center_y);
}

void Grid_Draw(grid_t* grid) {
    C2D_SpriteSetCenter(&grid->empty, 0.5f, 0.5f);

    for (s32 xx = 0; xx < grid->grid_w; xx++) {
        for (s32 yy = 0; yy < grid->grid_h; yy++) {
            C2D_SpriteSetPos(&grid->empty, 
                grid->center_x - (grid->grid_w * grid->tile_w) / 2 + grid->tile_w / 2 + xx * grid->tile_w / 2 + yy * grid->tile_w / 2,
                grid->center_y + yy * grid->tile_h / 2 - xx * grid->tile_h / 2
            );
            C2D_DrawSprite(&grid->empty);
        }
    }

    C2D_DrawSprite(&grid->debug_point_sprite);
}
