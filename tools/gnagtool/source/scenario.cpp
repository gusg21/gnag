//
// Created by Angus Goucher on 5/25/2024.
//

#include "scenario.h"

#include "cJSON.h"

constexpr uint32_t jsonBufferSize = 4096;

hazard_type_e Scenario::GetHazardTypeAtTile(int tileX, int tileY) {
    for (uint32_t hazardIndex = 0; hazardIndex < SCENARIO_MAX_HAZARDS; hazardIndex++) {
        hazard_data_t* hazard = &RawScenario.hazards[hazardIndex];
        if (hazard->initialized && hazard->tile_pos.x == (float)tileX && hazard->tile_pos.y == (float)tileY) {
            return hazard->type;
        }
    }
    return HAZARD_NONE;
}

void Scenario::SetHazardTypeAtTile(int tileX, int tileY, hazard_type_e type) {
    for (uint32_t hazardIndex = 0; hazardIndex < SCENARIO_MAX_HAZARDS; hazardIndex++) {
        hazard_data_t* hazard = &RawScenario.hazards[hazardIndex];
        if (hazard->initialized && hazard->tile_pos.x == (float)tileX && hazard->tile_pos.y == (float)tileY) {
            hazard->type = type;
            return;
        }
    }
    hazard_data_t* hazard = &RawScenario.hazards[m_NextHazardIndex];
    m_NextHazardIndex++;
}

bool Scenario::IsTileInRange(int tileX, int tileY) const {
    return !(tileX < 0 || tileX >= RawScenario.grid_width || tileY < 0 || tileY >= RawScenario.grid_height);
}

void Scenario::Resize(int width, int height) {
    RawScenario.grid_width = width;
    RawScenario.grid_height = height;
}

uint32_t Scenario::GetWidth() const { return RawScenario.grid_width; }

uint32_t Scenario::GetHeight() const { return RawScenario.grid_height; }

void Scenario::AddCharacterData(character_data_t characterData) {
    RawScenario.characters[m_NextCharacterIndex] = characterData;
    m_NextCharacterIndex++;
}

void Scenario::LoadFromJSON(const std::string& jsonFile) {
    Scenario_LoadFromJSON(&RawScenario, jsonFile.c_str());

    for (uint32_t hazardIndex = 0; hazardIndex < SCENARIO_MAX_HAZARDS; hazardIndex++) {
        hazard_data_t* hazard = &RawScenario.hazards[hazardIndex];
        if (!hazard->initialized) {
            m_NextHazardIndex = hazardIndex;
            break;
        }
    }

    for (uint32_t characterIndex = 0; characterIndex < SCENARIO_MAX_CHARACTERS; characterIndex++) {
        character_data_t* character = &RawScenario.characters[characterIndex];
        if (!character->initialized) {
            m_NextCharacterIndex = characterIndex;
            break;
        }
    }
}
