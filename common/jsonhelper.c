#include "jsonhelper.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "_defs.h"
#include "panic.h"

#ifdef __cplusplus
extern "C" {
#endif

cJSON* JSONHelper_LoadCJSONFromFile(const char* json_file) {
    // Get file size
    struct stat json_file_stat;
    stat(json_file, &json_file_stat);
    size_t json_file_size = json_file_stat.st_size;

    // Open the file
    FILE* fptr = fopen(json_file, "r");
    if (fptr == NULL) {
        CTR_PRINTF("Unable to open file %s\n", json_file);
        return NULL;
    }


    // Allocate the file buffer
    char* buffer = malloc(sizeof(char) * json_file_size + 1);
    if (buffer == NULL) {
        CTR_PRINTF("Unable to allocate memory for reading to JSON file %s\n", json_file);
        return NULL;
    }
    fread(buffer, sizeof(char), json_file_size, fptr);
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

void JSONHelper_WriteCJSONToFile(cJSON* json_object, const char* json_file) {
    // Open the file
    FILE* fptr = fopen(json_file, "w");
    if (fptr == NULL) {
        CTR_PRINTF("Unable to open file %s\n", json_file);
        return;
    }

    // Get the JSON size
    const char* json_string = cJSON_Print(json_object);
    size_t json_size = strlen(json_string);

    // Write the JSON buffer
    fwrite(json_string, sizeof(char), json_size, fptr);
    fclose(fptr);
}

#ifdef __cplusplus
}
#endif