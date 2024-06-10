#include "jsonhelper.h"

#include <stdio.h>
#include <stdlib.h>

#include "_defs.h"
#include "panic.h"

#ifdef __cplusplus
extern "C" {
#endif

cJSON* JSONHelper_LoadCJSONFromFile(const char* json_file, size_t max_file_buffer_size) {
    // Open the file
    FILE* fptr = fopen(json_file, "r");
    if (fptr == NULL) {
        CTR_PRINTF("Unable to open file %s\n", json_file);
        return NULL;
    }

    // Allocate the file buffer
    char* buffer = malloc(sizeof(char) * max_file_buffer_size);
    fread(buffer, sizeof(char), max_file_buffer_size, fptr);
    fclose(fptr);

    // Parse the JSON
    cJSON* json = cJSON_Parse(buffer);
    if (json == NULL) {
        Panic_Panic();
        size_t error_byte_offset = (size_t)cJSON_GetErrorPtr() - (size_t)buffer;
        char* begin = &buffer[MAX(error_byte_offset - 50, 0)];
        size_t length = 50;
        CTR_PRINTF("Error loading JSON from %s\n", json_file);
        CTR_PRINTF("%*.*s\n", (int)length, (int)length, begin);
        CTR_PRINTF("%zu bytes from start of file\n", error_byte_offset);
    }

    // Free the buffer
    free(buffer);

    return json;
}

#ifdef __cplusplus
}
#endif