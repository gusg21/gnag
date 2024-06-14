#ifndef JSONHELPER_H
#define JSONHELPER_H

#include "cJSON.h"

#ifdef __cplusplus
extern "C" {
#endif

cJSON* JSONHelper_LoadCJSONFromFile(const char* json_file);
void JSONHelper_WriteCJSONToFile(cJSON* json_object, const char* json_file);

#ifdef __cplusplus
}
#endif

#endif // JSONHELPER_H