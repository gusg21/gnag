#ifndef JSONHELPER_H
#define JSONHELPER_H

#include "cJSON.h"

#ifdef __cplusplus
extern "C" {
#endif

cJSON* JSONHelper_LoadCJSONFromFile(const char* json_file, size_t max_file_buffer_size);

#ifdef __cplusplus
}
#endif

#endif // JSONHELPER_H