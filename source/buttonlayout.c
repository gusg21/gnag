#include "buttonlayout.h"

#include <citro2d.h>

#include "cJSON.h"
#include "_defs.h"
#include "debugconsole.h"
#include "uisprites.h"

void ButtonLayout_InitFromFile(button_layout_t* button_layout, char filename[]) {
    memset(button_layout->datas, 0, sizeof(button_data_t) * 32);
    ButtonLayout_LoadNewLayoutFromFile(button_layout, filename);
}

void ButtonLayout_LoadNewLayoutFromFile(button_layout_t* button_layout, char filename[])
{
    FILE* fptr = fopen(filename, "r");
    if (fptr == NULL) { 
        DebugConsole_Print("Unable to open file", 20); 
    } 

    char buffer[4096];
    fread(buffer, sizeof(char), sizeof(buffer), fptr); 
    fclose(fptr);
  
    cJSON* json = cJSON_Parse(buffer);
    if (json == NULL)
    {
        DebugConsole_Print("Error loading JSON", 19);
    }
  
    cJSON* buttons = cJSON_GetObjectItem(json, "Buttons");
    cJSON* button;
    u32 i = 0;

    cJSON_ArrayForEach(button, buttons)
    {
        // Get & Set Position
        cJSON* pos = cJSON_GetObjectItem(button, "Pos");
        cJSON* x = cJSON_GetArrayItem(pos, 0);
        cJSON* y = cJSON_GetArrayItem(pos, 1);

        button_layout->datas[i].pos = (vec2_t){x->valueint, y->valueint};

        // Get & Set Size
        cJSON* size = cJSON_GetObjectItem(button, "Size");
        x = cJSON_GetArrayItem(size, 0);
        y = cJSON_GetArrayItem(size, 1);

        button_layout->datas[i].size = (vec2_t){x->valueint, y->valueint};

        // Get & Set Sprite Index
        cJSON* sprite_idx = cJSON_GetObjectItem(button, "Sprite Index");

        button_layout->datas[i].sprite_idx = sprite_idx->valueint;

        // Get & Set Color
        cJSON* color = cJSON_GetObjectItem(button, "Color");
        cJSON* r = cJSON_GetArrayItem(color, 0);
        cJSON* g = cJSON_GetArrayItem(color, 1);
        cJSON* b = cJSON_GetArrayItem(color, 2);
        cJSON* a = cJSON_GetArrayItem(color, 3);

        button_layout->datas[i].color = C2D_Color32f(r->valuedouble, g->valuedouble, b->valuedouble, a->valuedouble);

        // Get & Set Callback
        cJSON* callback = cJSON_GetObjectItem(button, "Callback");
        
        button_layout->datas[i].callback = callback->valueint;

        i++;
    }

    button_layout->data_count = i;

    cJSON_Delete(json);
}
