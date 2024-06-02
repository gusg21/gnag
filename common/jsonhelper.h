#ifndef JSONHELPER_H
#define JSONHELPER_H

#include "cJSON.h"

cJSON* JSONHelper_LoadCJSONFromFile(const char* json_file, size_t max_file_buffer_size);

#endif // JSONHELPER_H