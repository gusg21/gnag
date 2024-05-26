//
// Created by Angus Goucher on 5/25/2024.
//

#ifndef GNAGTOOL_SCENARIO_H
#define GNAGTOOL_SCENARIO_H


#include <string>
#include <vector>

struct Hazard {
    int TileX, TileY;
    int HazardType;
};

class Scenario {
public:
    Scenario(int gridWidth, int gridHeight);

    static Scenario LoadScenarioFromJSON(const std::string& jsonPath);

    int GridWidth = 0;
    int GridHeight = 0;
    std::vector<Hazard> Hazards {};
};


#endif //GNAGTOOL_SCENARIO_H
