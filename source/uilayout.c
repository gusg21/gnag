#include "uilayout.h"

#include <citro2d.h>
#include "stdlib.h"

#include "_defs.h"
#include "cJSON.h"
#include "debugconsole.h"
#include "panic.h"
#include "uisprites.h"

#define UILAYOUT_MAX_FILE_SIZE 8192

void UILayout_InitFromFile(ui_layout_t* layout, const char* filename) {
    memset(layout->button_datas, 0, sizeof(button_data_t) * 32);
    UILayout_LoadNewLayoutFromFile(layout, filename);
}

void UILayout_LoadNewLayoutFromFile(ui_layout_t* layout, const char* filename) {
    FILE* fptr = fopen(filename, "r");
    if (fptr == NULL) {
        Panic_Panic();
        printf("Unable to open file %s", filename);
    }

    char* buffer = malloc(sizeof(char) * UILAYOUT_MAX_FILE_SIZE);
    fread(buffer, sizeof(char), UILAYOUT_MAX_FILE_SIZE, fptr);
    fclose(fptr);

    cJSON* json = cJSON_Parse(buffer);
    if (json == NULL) {
        Panic_Panic();
        printf("Error loading JSON from %s\n", filename);
        printf("%*.*s\n", 100, 100, buffer);
        printf("%d bytes in", (size_t)cJSON_GetErrorPtr() - (size_t)buffer);
    }

    cJSON* buttons = cJSON_GetObjectItem(json, "Buttons");
    cJSON* button;

    cJSON_ArrayForEach(button, buttons) {
        // Get index
        cJSON* index_object = cJSON_GetObjectItem(button, "Index");
        u32 index = index_object->valueint;

        // Get & Set Position
        cJSON* pos = cJSON_GetObjectItem(button, "Pos");
        cJSON* x = cJSON_GetArrayItem(pos, 0);
        cJSON* y = cJSON_GetArrayItem(pos, 1);
        layout->button_datas[index].pos = (vec2_t){x->valueint, y->valueint};

        // Get & Set Size
        cJSON* size = cJSON_GetObjectItem(button, "Size");
        x = cJSON_GetArrayItem(size, 0);
        y = cJSON_GetArrayItem(size, 1);
        layout->button_datas[index].size = (vec2_t){x->valueint, y->valueint};

        // Get & Set Sprite Index
        cJSON* sprite_idx = cJSON_GetObjectItem(button, "Sprite Index");
        layout->button_datas[index].sprite_idx = sprite_idx->valueint;

        // Get & Set Color
        cJSON* color = cJSON_GetObjectItem(button, "Color");
        cJSON* r = cJSON_GetArrayItem(color, 0);
        cJSON* g = cJSON_GetArrayItem(color, 1);
        cJSON* b = cJSON_GetArrayItem(color, 2);
        cJSON* a = cJSON_GetArrayItem(color, 3);
        layout->button_datas[index].color =
            C2D_Color32f(r->valuedouble, g->valuedouble, b->valuedouble, a->valuedouble);

        // Get & Set Callback
        cJSON* callback = cJSON_GetObjectItem(button, "Callback");
        layout->button_datas[index].callback = callback->valueint;

        // Mark Button Data as initialized
        layout->button_datas[index].initialized = true;
    }

    cJSON_Delete(json);
    free(buffer);
}
