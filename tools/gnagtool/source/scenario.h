//
// Created by Angus Goucher on 5/25/2024.
//

#ifndef GNAGTOOL_SCENARIO_H
#define GNAGTOOL_SCENARIO_H


#include <string>
#include <vector>
#include "jsonserializable.h"

#include "hazarddata.h"
#include "characterdata.h"

struct HazardData {
    ::hazard_type_e HazardType;
};

struct CharacterData {
    int TileX;
    int TileY;
    ::character_type_e Type;
    bool IsPlayerControlled;
};

class Scenario : public JSONSerializable {
public:
    HazardData GetHazardDataAtTile(int tileX, int tileY);
    void SetHazardDataAtTile(int tileX, int tileY, HazardData data);
    void Resize(int width, int height);
    bool IsTileInRange(int tileX, int tileY) const;

protected:
    void FromJSON(cJSON *json) override;
    void ToJSON(cJSON *json) override;

public:
    int GridWidth = 10;
    int GridHeight = 10;
    std::vector<HazardData> Hazards { };
    std::vector<CharacterData> Characters { };
};


#endif //GNAGTOOL_SCENARIO_H
