#ifndef HAZARDDATA_H
#define HAZARDDATA_H

typedef enum {
    // None 0
    HAZARD_NONE,
    // Spikes 1-4
    HAZARD_SPIKES,
    HAZARD_FLAMING_SPIKES,
    HAZARD_FROZEN_SPIKES,
    HAZARD_ELECTRIC_SPIKES,
    // Water 5-7
    HAZARD_WATER,
    HAZARD_STEAM,
    HAZARD_ELECTRIC_WATER,
    // Fire 8
    HAZARD_FIRE,
    // Ice 9
    HAZARD_ICE,
    // Lightning 10
    HAZARD_LIGHTNING,
    // Wind 11
    HAZARD_GUST,

    HAZARD_COUNT
} hazard_type_e;

#endif // HAZARDDATA_H