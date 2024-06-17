#include "scenario.h"

#include <stdlib.h>
#include <string.h>

#include "cJSON.h"
#include "jsonhelper.h"

#ifdef __cplusplus
extern "C" {
#endif

void Scenario_LoadFromJSON(scenario_t* scenario, const char* json_path) {
    Scenario_Init(scenario);

    cJSON* scenario_json = JSONHelper_LoadCJSONFromFile(json_path);

    scenario->grid_width = (u32) cJSON_GetNumberValue(cJSON_GetObjectItem(scenario_json, "Grid Width"));
    scenario->grid_height = (u32) cJSON_GetNumberValue(cJSON_GetObjectItem(scenario_json, "Grid Height"));

    cJSON* hazards_json = cJSON_GetObjectItem(scenario_json, "Hazards");
    cJSON* hazard_json;
    u32 hazard_index = 0;
    cJSON_ArrayForEach(hazard_json, hazards_json) {
        hazard_data_t* hazard_data = &scenario->hazards[hazard_index];
        hazard_data->initialized = true;

        cJSON* hazard_tile_pos_json = cJSON_GetObjectItem(hazard_json, "Tile Pos");
        hazard_data->tile_pos.x = (float) cJSON_GetNumberValue(cJSON_GetArrayItem(hazard_tile_pos_json, 0));
        hazard_data->tile_pos.y = (float) cJSON_GetNumberValue(cJSON_GetArrayItem(hazard_tile_pos_json, 1));

        hazard_data->type = cJSON_GetNumberValue(cJSON_GetObjectItem(hazard_json, "Type"));

        hazard_index++;
    }

    cJSON* characters_json = cJSON_GetObjectItem(scenario_json, "Characters");
    cJSON* character_json;
    u32 character_index = 0;
    cJSON_ArrayForEach(character_json, characters_json) {
        character_data_t* character_data = &scenario->characters[character_index];
        character_data->initialized = true;

        cJSON* character_tile_pos_json = cJSON_GetObjectItem(character_json, "Tile Pos");
        character_data->tile_pos.x = (float) cJSON_GetNumberValue(cJSON_GetArrayItem(character_tile_pos_json, 0));
        character_data->tile_pos.y = (float) cJSON_GetNumberValue(cJSON_GetArrayItem(character_tile_pos_json, 1));

        character_data->type = cJSON_GetNumberValue(cJSON_GetObjectItem(character_json, "Type"));
        character_data->is_player_controlled = cJSON_IsTrue(cJSON_GetObjectItem(character_json, "Player Controlled"));

        character_index++;
    }

    cJSON_Delete(scenario_json);
}

void Scenario_Init(scenario_t* scenario) {
    memset(scenario->characters, 0, sizeof(character_data_t) * SCENARIO_MAX_CHARACTERS);
    memset(scenario->hazards, 0, sizeof(hazard_data_t) * SCENARIO_MAX_HAZARDS);

    scenario->grid_width = 10;
    scenario->grid_height = 10;
}

void Scenario_SaveToJSON(scenario_t* scenario, const char* json_path) {
    cJSON* scenario_json = cJSON_CreateObject();

    cJSON_AddNumberToObject(scenario_json, "Grid Width", scenario->grid_width);
    cJSON_AddNumberToObject(scenario_json, "Grid Height", scenario->grid_height);

    cJSON *hazardArray = cJSON_CreateArray();
    cJSON_AddItemToObject(scenario_json, "Hazards", hazardArray);
    for (uint32_t hazardIndex = 0; hazardIndex < SCENARIO_MAX_HAZARDS; hazardIndex++) {
        hazard_data_t* hazard = &scenario->hazards[hazardIndex];
        if (hazard->type != HAZARD_NONE && hazard->initialized) {
            uint32_t hazardPos[2] = {hazardIndex % scenario->grid_width, hazardIndex / scenario->grid_height};

            cJSON *hazard_json = cJSON_CreateObject();

            cJSON_AddNumberToObject(hazard_json, "Type", (int) hazard->type);

            cJSON_AddItemToObject(hazard_json, "Tile Pos", cJSON_CreateIntArray((int*) hazardPos, 2));

            cJSON_AddItemToArray(hazardArray, hazard_json);
        }
    }

    cJSON *character_array = cJSON_CreateArray();
    cJSON_AddItemToObject(scenario_json, "Characters", character_array);
    for (uint32_t character_index = 0; character_index < SCENARIO_MAX_CHARACTERS; character_index++) {
        character_data_t* character = &scenario->characters[character_index];

        if (character->initialized) {
            cJSON* character_json = cJSON_CreateObject();

            cJSON_AddNumberToObject(character_json, "Type", (int)character->type);
            cJSON_AddBoolToObject(character_json, "Player Controlled", character->is_player_controlled);
            cJSON_AddItemToObject(character_json, "Tile Pos", cJSON_CreateFloatArray(&character->tile_pos.x, 2));

            cJSON_AddItemToArray(character_array, character_json);
        }
    }

    JSONHelper_WriteCJSONToFile(scenario_json, json_path);
}

#ifdef __cplusplus
}
#endif