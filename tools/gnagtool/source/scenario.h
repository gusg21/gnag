//
// Created by Angus Goucher on 5/25/2024.
//

#ifndef GNAGTOOL_SCENARIO_H
#define GNAGTOOL_SCENARIO_H


#include <string>
#include <vector>

enum class HazardType {
    NONE = 0,
    SPIKES,
    WATER,
    COUNT
};

struct HazardData {
    HazardType HazardType;
};

enum class CharacterType {
    NONE = 0,
    GOOD,
    BAD,
    UGLY,
    ENEMY,
    COUNT
};

struct CharacterData {
    int TileX;
    int TileY;
    CharacterType Type;
    bool IsPlayerControlled;
};

class Scenario {
public:
    Scenario(int gridWidth, int gridHeight);

    HazardData GetHazardDataAtTile(int tileX, int tileY);
    void SetHazardDataAtTile(int tileX, int tileY, HazardData data);
    void Resize(int width, int height);

    static Scenario LoadScenarioFromJSON(const std::string& jsonPath);
    void SaveToJSON(const std::string& jsonPath);

    int GridWidth = 0;
    int GridHeight = 0;
    std::vector<HazardData> Hazards {};
    std::vector<CharacterData> Characters {};

    bool IsTileInRange(int tileX, int tileY) const;
};


#endif //GNAGTOOL_SCENARIO_H
