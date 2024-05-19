// Useful macro defines

#ifndef DEFS_H
#define DEFS_H

#define CTR_PRINTF(fmt, ...) fprintf(stderr, "[GNAG-INFO] " fmt __VA_OPT__(,) __VA_ARGS__)

#define TOP_SCREEN_WIDTH  400
#define TOP_SCREEN_HEIGHT 240
#define BOTTOM_SCREEN_WIDTH  320
#define BOTTOM_SCREEN_HEIGHT 240

#define GAME_WORLD_CENTER_X 500
#define GAME_WORLD_CENTER_Y 500

#endif // DEFS_H