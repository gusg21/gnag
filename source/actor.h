#ifndef ACTOR_H
#define ACTOR_H

#include "c2d/sprite.h"

typedef struct
{
    bool alive;
    C2D_Sprite sprite;
    float x, y;
} actor_t;


#endif // ACTOR_H