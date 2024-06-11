#include "hazarddata.h"

#include "_defs.h"
#include "sprites.h"

#ifdef __cplusplus
extern "C" {
#endif

u32 HazardData_GetSpriteIndexForHazardType(hazard_type_e type) {
    switch (type) {
        case HAZARD_SPIKES:
            return sprites_spikes_idx;
        case HAZARD_WATER:
            return sprites_watertile_idx;

        default:
            return sprites_emptytile_idx;
    }
}

#ifdef __cplusplus
}
#endif