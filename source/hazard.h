#ifndef HAZARD_H
#define HAZARD_H

#include <3ds.h>

#include <citro2d.h>

#include "vec2.h"
#include "grid.h"
#include "hazarddata.h"

typedef struct {
    bool initialized;

    vec2_t tile_pos;
    hazard_type_e type;
} hazard_t;

void Hazard_Init(hazard_t* hazard, float tile_x, float tile_y, hazard_type_e type);
void Hazard_Draw(hazard_t* hazard, C2D_SpriteSheet sheet, grid_t* grid);

hazard_type_e Hazard_Combine(hazard_type_e hazard_type_1, hazard_type_e hazard_type_2);

#endif // HAZARD_H