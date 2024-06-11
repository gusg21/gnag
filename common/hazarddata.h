#ifndef HAZARDDATA_H
#define HAZARDDATA_H

#include "_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    HAZARD_NONE,
    HAZARD_SPIKES,
    HAZARD_WATER,
    HAZARD_COUNT
} hazard_type_e;

u32 HazardData_GetSpriteIndexForHazardType(hazard_type_e type);

#ifdef __cplusplus
}
#endif

#endif // HAZARDDATA_H