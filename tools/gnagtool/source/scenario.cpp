//
// Created by Angus Goucher on 5/25/2024.
//

#include "scenario.h"

#include "cJSON.h"

Scenario::Scenario(int gridWidth, int gridHeight) : GridWidth(gridWidth), GridHeight(gridHeight) {}

Scenario Scenario::LoadScenarioFromJSON(const std::string &jsonPath) {
    // Load the file
    constexpr uint32_t jsonBufferSize = 4096;
    char jsonBuffer[jsonBufferSize] = {0};
    FILE *jsonFile;
    fopen_s(&jsonFile, jsonPath.c_str(), "r");
    fread(jsonBuffer, sizeof(char), jsonBufferSize, jsonFile);
    fclose(jsonFile);

    // Parse the JSON
    cJSON *scenarioJson = cJSON_Parse(jsonBuffer);
    int gridWidth = (int) cJSON_GetNumberValue(cJSON_GetObjectItem(scenarioJson, "Grid Width"));
    int gridHeight = (int) cJSON_GetNumberValue(cJSON_GetObjectItem(scenarioJson, "Grid Height"));

    Scenario scenario = Scenario{gridWidth, gridHeight};

    // Load hazards
    cJSON *hazardArray = cJSON_GetObjectItem(scenarioJson, "Hazards");
    cJSON *hazardJSON;
    cJSON_ArrayForEach(hazardJSON, hazardArray) {
        cJSON* tilePos = cJSON_GetObjectItem(hazardJSON, "Tile Pos");
        int tileX = (int) cJSON_GetNumberValue(cJSON_GetArrayItem(tilePos, 0));
        int tileY = (int) cJSON_GetNumberValue(cJSON_GetArrayItem(tilePos, 1));

        int hazardType = (int) cJSON_GetNumberValue(cJSON_GetObjectItem(hazardJSON, "Type"));

        Hazard hazard {};
        hazard.HazardType = hazardType;
        hazard.TileX = tileX;
        hazard.TileY = tileY;
        scenario.Hazards.push_back(hazard);
    }

    return scenario;
}
