//
// Created by Angus Goucher on 6/1/2024.
//

#include "jsonserializable.h"
#include "cJSON.h"

constexpr uint32_t jsonBufferSize = 65536;

void JSONSerializable::LoadFromJSON(const std::string &jsonPath) {
    // Open the file
    char jsonBuffer[jsonBufferSize] = { 0 };
    FILE* jsonFile;
    fopen_s(&jsonFile, jsonPath.c_str(), "r");
    if (jsonFile == nullptr) {
        printf("Failed to open file %s\n", jsonPath.c_str());
        return;
    }

    // Read the data
    fread(jsonBuffer, sizeof(char), jsonBufferSize, jsonFile);
    fclose(jsonFile);
    jsonFile = nullptr;

    // Parse the JSON
    cJSON* json = cJSON_ParseWithLength(jsonBuffer, jsonBufferSize);

    // Load
    FromJSON(json);

    // Cleanup
    cJSON_Delete(json);
}

void JSONSerializable::SaveToJSON(const std::string &jsonPath) {
    cJSON* json = cJSON_CreateObject();
    ToJSON(json);

    // Open the file
    char jsonBuffer[jsonBufferSize] = { 0 };
    FILE* jsonFile;
    fopen_s(&jsonFile, jsonPath.c_str(), "w");
    if (jsonFile == nullptr) {
        printf("Failed to open file %s\n", jsonPath.c_str());
        return;
    }

    // Serialize JSON
    cJSON_PrintPreallocated(json, jsonBuffer, jsonBufferSize, true);

    // Write to file
    fwrite(jsonBuffer, sizeof(char), jsonBufferSize, jsonFile);
    fclose(jsonFile);

    // Cleanup
    cJSON_Delete(json);
}
