//
// Created by Angus Goucher on 5/25/2024.
//

#ifndef GNAGTOOL_SCENARIO_H
#define GNAGTOOL_SCENARIO_H


#include <string>

class Scenario {
public:
    Scenario(int gridHeight, int gridWidth);
    Scenario(Scenario&) = default; // We DO want a copy constructor
    Scenario& operator=(Scenario&) = delete;

    static Scenario LoadScenarioFromJSON(const std::string* jsonPath);

    int GridWidth = 0;
    int GridHeight = 0;
};


#endif //GNAGTOOL_SCENARIO_H
