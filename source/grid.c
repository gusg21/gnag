#include "grid.h"

#include "debugconsole.h"
#include "sprites.h"

void Grid_Init(grid_t* grid, C2D_SpriteSheet sheet, size_t empty_index, size_t spikes_index, u32 grid_w, u32 grid_h,
               s32 center_x, s32 center_y) {
    C2D_SpriteFromSheet(&grid->empty, sheet, empty_index);
    C2D_SpriteSetCenter(&grid->empty, 0.5f, 0.75f);

    C2D_SpriteFromSheet(&grid->spikes, sheet, spikes_index);
    C2D_SpriteSetCenter(&grid->spikes, 0.5f, 0.75f);

    grid->tile_w = 64;
    grid->tile_h = 32;

    grid->grid_w = grid_w;
    grid->grid_h = grid_h;

    grid->center_x = center_x;
    grid->center_y = center_y;

    memset(grid->tile_data, 0, GRID_MAX_TILE_DATAS);

    C2D_SpriteFromSheet(&grid->debug_point_sprite, sheet, sprites_point_idx);
    C2D_SpriteSetCenter(&grid->debug_point_sprite, 0.5f, 0.5f);
    C2D_SpriteSetPos(&grid->debug_point_sprite, grid->center_x, grid->center_y);
}

void Grid_Draw(grid_t* grid) {
    for (s32 xx = 0; xx < grid->grid_w; xx++) {
        for (s32 yy = 0; yy < grid->grid_h; yy++) {
            grid_tile_t* tile = Grid_GetTileAt(grid, xx, yy);
            C2D_Sprite* tile_sprite = Grid_GetSpriteForTile(grid, tile);
            float tile_depth = Grid_GetIsoDepthForTilePos(grid, xx, yy);
            vec2_t world_pos = Grid_GridPosToWorldPos(grid, (vec2i_t){.x = xx, .y = yy});
            C2D_SpriteSetDepth(tile_sprite, tile_depth);
            C2D_SpriteSetPos(tile_sprite, world_pos.x, world_pos.y);
            C2D_DrawSprite(tile_sprite);

            char buf[100] = {0};
            snprintf(buf, 100, "%.2f (x=%ld y=%ld)", tile_depth, xx, yy);
            //DebugConsole_Print(buf, 100);
        }
    }

    C2D_DrawSprite(&grid->debug_point_sprite);
}

C2D_Sprite* Grid_GetSpriteForTile(grid_t* grid, grid_tile_t* tile) {
    if (tile->is_spikes) return &grid->spikes;
    return &grid->empty;
}

grid_tile_t* Grid_GetTileAt(grid_t* grid, u32 tile_x, u32 tile_y) {
    return &grid->tile_data[tile_x + tile_y * grid->grid_w];
}

vec2_t Grid_GridPosToWorldPos(grid_t* grid, vec2i_t grid_pos) {
    return (vec2_t){.x = grid->center_x - (grid->grid_w * grid->tile_w) / 2 + grid->tile_w / 2 +
                         grid_pos.x * grid->tile_w / 2 + grid_pos.y * grid->tile_w / 2,
                    .y = grid->center_y + grid_pos.y * grid->tile_h / 2 - grid_pos.x * grid->tile_h / 2};
}

float Grid_GetIsoDepthForTilePos(grid_t* grid, u32 tile_x, u32 tile_y) {
    // return ((((tile_y - tile_x) / ((float)(grid->tile_w) * 50.f))) * -1.f + 0.1f);

    return (tile_y - tile_x) + (float)(grid->tile_w);
}
