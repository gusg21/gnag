// Useful macro defines

#ifndef DEFS_H
#define DEFS_H

#ifndef __3DS__
#include <stdbool.h>
#include <stdint.h>
typedef uint8_t u8;
typedef uint32_t u32;
typedef int32_t s32;
#define CTR_PRINTF printf
#else
#include <3ds.h>
#define CTR_PRINTF(fmt, ...) fprintf(stderr, "[GNAG-INFO] " fmt __VA_OPT__(,) __VA_ARGS__)
#endif


#define UNUSED(x) (void)(x)

#define MIN(x,y) ((x) < (y)) ? (x) : (y)
#define MAX(x,y) ((x) > (y)) ? (x) : (y)
#define CLAMP(x,min,max) MIN(MAX((x), (min)), (max))

#define TOP_SCREEN_WIDTH  400
#define TOP_SCREEN_HEIGHT 240
#define BOTTOM_SCREEN_WIDTH  320
#define BOTTOM_SCREEN_HEIGHT 240

#define CLEAR_COLOR_INTS 223, 246, 245, 255

#define GAME_WORLD_CENTER_X 500
#define GAME_WORLD_CENTER_Y 500

#endif // DEFS_H