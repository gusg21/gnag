//
// Created by Angus Goucher on 6/1/2024.
//

#ifndef GNAGTOOL_JSONSERIALIZABLE_H
#define GNAGTOOL_JSONSERIALIZABLE_H

#include <string>
#include "cJSON.h"

class JSONSerializable {
public:
    void LoadFromJSON(const std::string& jsonPath);
    void SaveToJSON(const std::string& jsonPath);

protected:
    virtual void FromJSON(cJSON* json) = 0;
    virtual void ToJSON(cJSON* json) = 0;
};


#endif // GNAGTOOL_JSONSERIALIZABLE_H
