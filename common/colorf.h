#ifndef COLORF_H
#define COLORF_H

#include "_defs.h"

typedef struct {
    float r, g, b, a;
} colorf_t;

#ifdef __3DS__
#include <3ds.h>
#define ColorF_ToU32(color) C2D_Color32f((color).r, (color).g, (color).b, (color).a)
#else
#define ColorF_ToU32(color) 0
#endif

#endif // COLORF_H