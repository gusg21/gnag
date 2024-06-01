#include "jsonhelper.h"

#include <stdio.h>
#include <stdlib.h>

#include "_defs.h"
#include "panic.h"

cJSON* JSONHelper_LoadCJSONFromFile(const char* json_file, size_t max_file_buffer_size) {
    // Open the file
    FILE* fptr = fopen(json_file, "r");
    if (fptr == NULL) {
        Panic_Panic();
        CTR_PRINTF("Unable to open file %s\n", json_file);
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
        CTR_PRINTF("%*.*s\n", length, length, begin);
        CTR_PRINTF("%d bytes from start of file\n", error_byte_offset);
    }

    // Free the buffer
    free(buffer);

    return json;
}