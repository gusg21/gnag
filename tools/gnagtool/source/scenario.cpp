//
// Created by Angus Goucher on 5/25/2024.
//

#include "scenario.h"

#include "cJSON.h"

constexpr uint32_t jsonBufferSize = 4096;

Scenario::Scenario(int gridWidth, int gridHeight) : GridWidth(gridWidth), GridHeight(gridHeight) {
    Hazards.resize(gridWidth * gridHeight);
}

Scenario Scenario::LoadScenarioFromJSON(const std::string &jsonPath) {
    // Load the file
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
        cJSON *tilePos = cJSON_GetObjectItem(hazardJSON, "Tile Pos");
        int tileX = (int) cJSON_GetNumberValue(cJSON_GetArrayItem(tilePos, 0));
        int tileY = (int) cJSON_GetNumberValue(cJSON_GetArrayItem(tilePos, 1));

        int hazardType = (int) cJSON_GetNumberValue(cJSON_GetObjectItem(hazardJSON, "Type"));

        scenario.SetHazardDataAtTile(tileX, tileY, HazardData{
                static_cast<HazardType>(hazardType)
        });
    }

    return scenario;
}

HazardData Scenario::GetHazardDataAtTile(int tileX, int tileY) {
    if (!IsTileInRange(tileX, tileY)) {
        return HazardData{
                HazardType::NONE
        };
    }
    return Hazards[tileX + tileY * GridWidth];
}

void Scenario::SetHazardDataAtTile(int tileX, int tileY, HazardData data) {
    if (!IsTileInRange(tileX, tileY)) {
        return;
    }
    Hazards[tileX + tileY * GridWidth] = data;
}

bool Scenario::IsTileInRange(int tileX, int tileY) const {
    return !(tileX < 0 || tileX >= GridWidth || tileY < 0 || tileY >= GridHeight);
}

void Scenario::Resize(int width, int height) {
    GridWidth = width;
    GridHeight = height;

    Hazards.resize(width * height);
}

void Scenario::SaveToJSON(const std::string &jsonPath) {
    cJSON *scenarioJson = cJSON_CreateObject();

    cJSON_AddNumberToObject(scenarioJson, "Grid Width", GridWidth);
    cJSON_AddNumberToObject(scenarioJson, "Grid Height", GridHeight);

    cJSON *hazardArray = cJSON_CreateArray();
    cJSON_AddItemToObject(scenarioJson, "Hazards", hazardArray);
    for (uint32_t hazardIndex = 0; hazardIndex < GridWidth * GridHeight; hazardIndex++) {
        HazardData hazard = Hazards[hazardIndex];
        if (hazard.HazardType != HazardType::NONE) {
            uint32_t hazardX = hazardIndex % GridWidth;
            uint32_t hazardY = hazardIndex / GridHeight;

            cJSON *hazardJson = cJSON_CreateObject();
            cJSON *hazardTilePosJson = cJSON_AddArrayToObject(hazardJson, "Tile Pos");

            cJSON_AddNumberToObject(hazardJson, "Type", (int) hazard.HazardType);

            cJSON_AddItemToArray(hazardTilePosJson, cJSON_CreateNumber(hazardX));
            cJSON_AddItemToArray(hazardTilePosJson, cJSON_CreateNumber(hazardY));

            cJSON_AddItemToArray(hazardArray, hazardJson);
        }
    }

    char jsonBuffer[jsonBufferSize] = {0};
    cJSON_PrintPreallocated(scenarioJson, jsonBuffer, jsonBufferSize, true);
    FILE* jsonFile;
    fopen_s(&jsonFile, jsonPath.c_str(), "w");
    fwrite(jsonBuffer, sizeof(char), strlen(jsonBuffer), jsonFile);
    fclose(jsonFile);
}
