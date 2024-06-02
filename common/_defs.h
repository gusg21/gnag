// Useful macro defines

#ifndef DEFS_H
#define DEFS_H

#ifndef __3DS__
#include <stdbool.h>
typedef uint8_t u8;
typedef uint32_t u32;
typedef int32_t s32;
#else
#include <3ds.h>
#endif

#define CTR_PRINTF(fmt, ...) fprintf(stderr, "[GNAG-INFO] " fmt __VA_OPT__(,) __VA_ARGS__)

#define UNUSED(x) (void)(x)

#define MIN(x,y) (x < y) ? x : y
#define MAX(x,y) (x > y) ? x : y
#define CLAMP(x,min,max) MIN(MAX(x, min), max)

#define TOP_SCREEN_WIDTH  400
#define TOP_SCREEN_HEIGHT 240
#define BOTTOM_SCREEN_WIDTH  320
#define BOTTOM_SCREEN_HEIGHT 240

#define GAME_WORLD_CENTER_X 500
#define GAME_WORLD_CENTER_Y 500

#endif // DEFS_H