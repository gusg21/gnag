#ifndef SCENARIO_H
#define SCENARIO_H

#define SCENARIO_MAX_JSON_LENGTH 4096
#define SCENARIO_MAX_CHARACTERS 64
#define SCENARIO_MAX_HAZARDS 64

#include "hazard.h"
#include "vec2.h"
#include "character.h"

typedef struct {
    bool initialized;

    vec2_t tile_pos;
    hazard_type_e type;
} hazard_data_t;

typedef struct {
    bool initialized;

    vec2_t tile_pos;
    character_type_e type;
    bool is_player_controlled;
} character_data_t;

typedef struct {
    hazard_data_t hazards[SCENARIO_MAX_HAZARDS];
    character_data_t characters[SCENARIO_MAX_CHARACTERS];

    u32 grid_width, grid_height;
} scenario_t;

void Scenario_LoadFromJSON(scenario_t* scenario, const char* json_path);

#endif // SCENARIO_H