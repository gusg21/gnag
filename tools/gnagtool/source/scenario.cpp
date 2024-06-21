//
// Created by Angus Goucher on 5/25/2024.
//

#include "scenario.h"

#include "cJSON.h"
#include "gnagoswrapper.h"

constexpr uint32_t jsonBufferSize = 4096;

static int floorf2i(float f) { return static_cast<int>(floorf(f)); }

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
        if (floorf2i(hazard->tile_pos.x) == tileX && floorf2i(hazard->tile_pos.y) == tileY) {
            if (hazard->initialized && type != HAZARD_NONE) {
                hazard->type = type;
                return;
            }

            if (type == HAZARD_NONE) {
                hazard->initialized = false;
                hazard->type = HAZARD_NONE;
                hazard->tile_pos = vec2_t{0.f, 0.f};
                return;
            }
        }
    }

    if (type != HAZARD_NONE) {
        hazard_data_t* hazard = &RawScenario.hazards[m_NextHazardIndex];
        hazard->initialized = true;
        hazard->type = type;
        hazard->tile_pos.x = static_cast<float>(tileX);
        hazard->tile_pos.y = static_cast<float>(tileY);
        m_NextHazardIndex++;
    }
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

uint32_t Scenario::GetHazardCount() {
    uint32_t hazardCount = 0;
    for (uint32_t hazardIndex = 0; hazardIndex < SCENARIO_MAX_HAZARDS; hazardIndex++) {
        if (RawScenario.hazards[hazardIndex].initialized) {
            hazardCount++;
        }
    }
    return hazardCount;
}

uint32_t Scenario::GetMaxHazardCount() { return SCENARIO_MAX_HAZARDS; }

uint32_t Scenario::GetCharacterCount() {
    uint32_t characterCount = 0;
    for (uint32_t characterIndex = 0; characterIndex < SCENARIO_MAX_CHARACTERS; characterIndex++) {
        if (RawScenario.characters[characterIndex].initialized) {
            characterCount++;
        }    
    }
    return characterCount;
}

uint32_t Scenario::GetMaxCharacterCount() { return SCENARIO_MAX_CHARACTERS; }

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
