#include "grid.h"

#include "_defs.h"
#include "panic.h"
#include "sprites.h"

void Grid_Init(grid_t* grid, C2D_SpriteSheet sheet, u32 grid_w, u32 grid_h, s32 center_x, s32 center_y) {
    grid->tile_w = 64;
    grid->tile_h = 32;

    grid->grid_w = grid_w;
    grid->grid_h = grid_h;

    grid->center_x = center_x;
    grid->center_y = center_y;

    grid->tile_sprite_sheet = sheet;

    memset(grid->tile_data, 0, sizeof(grid_tile_t) * GRID_MAX_TILE_DATAS);
    memset(grid->tile_sprites, 0, sizeof(C2D_Sprite) * GRID_MAX_TILE_DATAS);
}

void Grid_Build(grid_t* grid) {
    for (s32 xx = 0; xx < grid->grid_w; xx++) {
        for (s32 yy = 0; yy < grid->grid_h; yy++) {
            C2D_Sprite* sprite = Grid_GetSpriteAt(grid, xx, yy);
            grid_tile_t* tile = Grid_GetTileAt(grid, xx, yy);
            C2D_SpriteFromSheet(sprite, grid->tile_sprite_sheet, Grid_GetSpriteIndexForTile(grid, tile));
            C2D_SpriteSetCenter(sprite, 0.5f, 0.75f);
        }
    }
}

void Grid_Draw(grid_t* grid) {
    int start_x = grid->grid_w - 1;
    int start_y = 0;
    int x = start_x;
    int y = start_y;

    while (true) {
        while (x < grid->grid_w && y < grid->grid_h) {
            // visit
            C2D_Sprite* tile_sprite = Grid_GetSpriteAt(grid, x, y);
            vec2_t world_pos = Grid_GridPosToWorldPos(grid, (vec2i_t){.x = x, .y = y});
            C2D_SpriteSetPos(tile_sprite, world_pos.x, world_pos.y);
            C2D_DrawSprite(tile_sprite);

            // move dr
            x ++;
            y ++;
        }

        if (start_x > 0) {
            start_x --;
        } else {
            start_y ++;
        }

        if (start_y >= grid->grid_h) {
            break;
        }

        x = start_x;
        y = start_y;
    }
}

size_t Grid_GetSpriteIndexForTile(grid_t* grid, grid_tile_t* tile) {
    if (tile->is_spikes) return sprites_spikes_idx;
    return sprites_emptytile_idx;
}

grid_tile_t* Grid_GetTileAt(grid_t* grid, u32 tile_x, u32 tile_y) {
    return &grid->tile_data[tile_x + tile_y * grid->grid_w];
}

C2D_Sprite* Grid_GetSpriteAt(grid_t* grid, u32 tile_x, u32 tile_y) { return &grid->tile_sprites[tile_x + tile_y * grid->grid_w]; }

vec2_t Grid_GridPosToWorldPos(grid_t* grid, vec2i_t grid_pos) {
    return (vec2_t){.x = grid->center_x - (grid->grid_w * grid->tile_w) / 2 + grid->tile_w / 2 +
                         grid_pos.x * grid->tile_w / 2 + grid_pos.y * grid->tile_w / 2,
                    .y = grid->center_y + grid_pos.y * grid->tile_h / 2 - grid_pos.x * grid->tile_h / 2};
}

float Grid_GetIsoDepthForTilePos(grid_t* grid, u32 tile_x, u32 tile_y) {
    // return ((((tile_y - tile_x) / ((float)(grid->tile_w) * 50.f))) * -1.f + 0.1f);

    float depth = ((float)((s32)tile_y - (s32)tile_x)) / ((float)(grid->grid_w));

    if (depth > 1.f || depth < -1.f) {
        Panic_Panic();
        CTR_PRINTF("Tile depth OOB: %.2f %ld %ld\n", depth, tile_x, tile_y);
    }

    return depth;
}
