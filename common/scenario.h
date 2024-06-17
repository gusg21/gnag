#ifndef SCENARIO_H
#define SCENARIO_H

#define SCENARIO_MAX_JSON_LENGTH 65536
#define SCENARIO_MAX_CHARACTERS 64
#define SCENARIO_MAX_HAZARDS 64

#include "characterdata.h"
#include "hazarddata.h"
#include "vec2.h"

#ifdef __cplusplus
extern "C" {
#endif

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

void Scenario_Init(scenario_t* scenario);
void Scenario_LoadFromJSON(scenario_t* scenario, const char* json_path);
void Scenario_SaveToJSON(scenario_t* scenario, const char* json_path);

#ifdef __cplusplus
}
#endif

#endif  // SCENARIO_H