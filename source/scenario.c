#include "scenario.h"

#include <stdlib.h>

#include "cJSON.h"
#include "jsonhelper.h"

void Scenario_LoadFromJSON(scenario_t* scenario, const char* json_path) {
    cJSON* scenario_json = JSONHelper_LoadCJSONFromFile(json_path, SCENARIO_MAX_JSON_LENGTH);

    scenario->grid_width = cJSON_GetNumberValue(cJSON_GetObjectItem(scenario_json, "Grid Width"));
    scenario->grid_height = cJSON_GetNumberValue(cJSON_GetObjectItem(scenario_json, "Grid Height"));

    cJSON* hazards_json = cJSON_GetObjectItem(scenario_json, "Hazards");
    cJSON* hazard_json;
    u32 hazard_index = 0;
    cJSON_ArrayForEach(hazard_json, hazards_json) {
        hazard_data_t* hazard_data = &scenario->hazards[hazard_index];
        hazard_data->initialized = true;

        cJSON* hazard_tile_pos_json = cJSON_GetObjectItem(hazard_json, "Tile Pos");
        hazard_data->tile_pos.x = cJSON_GetNumberValue(cJSON_GetArrayItem(hazard_tile_pos_json, 0));
        hazard_data->tile_pos.y = cJSON_GetNumberValue(cJSON_GetArrayItem(hazard_tile_pos_json, 1));

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
        character_data->tile_pos.x = cJSON_GetNumberValue(cJSON_GetArrayItem(character_tile_pos_json, 0));
        character_data->tile_pos.y = cJSON_GetNumberValue(cJSON_GetArrayItem(character_tile_pos_json, 1));

        character_data->type = cJSON_GetNumberValue(cJSON_GetObjectItem(character_json, "Type"));
        character_data->is_player_controlled = cJSON_IsTrue(cJSON_GetObjectItem(character_json, "Player Controlled"));

        character_index++;
    }

    cJSON_Delete(scenario_json);
}