//
// Created by Angus Goucher on 5/25/2024.
//

#include "scenario.h"

Scenario::Scenario(int gridWidth, int gridHeight) : GridWidth(gridWidth), GridHeight(gridHeight) {}

Scenario Scenario::LoadScenarioFromJSON(const std::string *jsonPath) {
    return Scenario(0, 0);
}
