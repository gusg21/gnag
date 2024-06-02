//
// Created by Angus Goucher on 5/25/2024.
//

#include "scenario.h"

#include "cJSON.h"

constexpr uint32_t jsonBufferSize = 4096;

HazardData Scenario::GetHazardDataAtTile(int tileX, int tileY) {
    if (!IsTileInRange(tileX, tileY)) {
        return HazardData {
                HAZARD_NONE };
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

void Scenario::FromJSON(cJSON *json) {
    GridWidth = (int) cJSON_GetNumberValue(cJSON_GetObjectItem(json, "Grid Width"));
    GridHeight = (int) cJSON_GetNumberValue(cJSON_GetObjectItem(json, "Grid Height"));
    Resize(GridWidth, GridHeight);

    // Load hazards
    cJSON *hazardArray = cJSON_GetObjectItem(json, "Hazards");
    cJSON *hazardJson;
    cJSON_ArrayForEach(hazardJson, hazardArray) {
        cJSON *tilePos = cJSON_GetObjectItem(hazardJson, "Tile Pos");
        int tileX = (int) cJSON_GetNumberValue(cJSON_GetArrayItem(tilePos, 0));
        int tileY = (int) cJSON_GetNumberValue(cJSON_GetArrayItem(tilePos, 1));

        int hazardType = (int) cJSON_GetNumberValue(cJSON_GetObjectItem(hazardJson, "Type"));

        SetHazardDataAtTile(tileX, tileY, HazardData { static_cast<::hazard_type_e>(hazardType) });
    }

    // Load characters
    cJSON *characterArray = cJSON_GetObjectItem(json, "Characters");
    cJSON *characterJson;
    cJSON_ArrayForEach(characterJson, characterArray) {
        cJSON *tilePos = cJSON_GetObjectItem(characterJson, "Tile Pos");
        int tileX = (int) cJSON_GetNumberValue(cJSON_GetArrayItem(tilePos, 0));
        int tileY = (int) cJSON_GetNumberValue(cJSON_GetArrayItem(tilePos, 1));

        int characterType = (int) cJSON_GetNumberValue(cJSON_GetObjectItem(characterJson, "Type"));
        bool playerControlled = cJSON_IsTrue(cJSON_GetObjectItem(characterJson, "Player Controlled"));

        Characters.push_back(CharacterData {
                tileX, tileY, static_cast<character_type_e>(characterType), playerControlled });
    }
}

void Scenario::ToJSON(cJSON *json) {

}