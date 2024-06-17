//
// Created by Angus Goucher on 5/25/2024.
//

#ifndef GNAGTOOL_SCENARIO_H
#define GNAGTOOL_SCENARIO_H


#include <string>
#include <vector>
#include <scenario.h>
#include "jsonserializable.h"

#include "hazarddata.h"
#include "characterdata.h"

class Scenario {
public:
    void LoadFromJSON(const std::string& jsonFile);

    hazard_type_e GetHazardTypeAtTile(int tileX, int tileY);
    void SetHazardTypeAtTile(int tileX, int tileY, hazard_type_e type);
    void Resize(int width, int height);
    bool IsTileInRange(int tileX, int tileY) const;
    void AddCharacterData(character_data_t characterData);

    uint32_t GetWidth() const;
    uint32_t GetHeight() const;
public:
    scenario_t RawScenario;

private:
    uint32_t m_NextHazardIndex;
    uint32_t m_NextCharacterIndex;
};


#endif //GNAGTOOL_SCENARIO_H
