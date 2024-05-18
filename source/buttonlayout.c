#include "buttonlayout.h"

#include <citro2d.h>

#include "cJSON.h"
#include "_defs.h"
#include "debugconsole.h"
#include "uisprites.h"

void ButtonLayout_InitFromFile(button_layout_t* button_layout, char filename[]) {
    ButtonLayout_LoadNewLayoutFromFile(button_layout, filename);
}

void ButtonLayout_LoadNewLayoutFromFile(button_layout_t* button_layout, char filename[])
{
    FILE* fptr = fopen(filename, "r");
    if (fptr == NULL) { 
        DebugConsole_Print("Unable to open file", 20); 
    } 

    char buffer[2048]; 
    fread(buffer, sizeof(char), sizeof(buffer), fptr); 
    fclose(fptr);
  
    cJSON* json = cJSON_Parse(buffer);
    if (json == NULL)
    {
        DebugConsole_Print("Error loading JSON", 19);
    }
  
    // access the JSON data 
    cJSON* count = cJSON_GetObjectItem(json, "Button Count");
    button_layout->data_count = count->valueint;

    cJSON* buttons = cJSON_GetObjectItem(json, "Buttons");
    cJSON* button;
    u32 i = 0;

    cJSON_ArrayForEach(button, buttons)
    {
        cJSON* pos = cJSON_GetObjectItem(button, "Pos");
        cJSON* size = cJSON_GetObjectItem(button, "Size");
        cJSON* sprite_idx = cJSON_GetObjectItem(button, "Sprite Index");
        cJSON* color = cJSON_GetObjectItem(button, "Color");
        cJSON* callback = cJSON_GetObjectItem(button, "Callback");

        button_layout->datas[i].pos = (vec2_t){pos[0].valuedouble, pos[1].valuedouble};
        button_layout->datas[i].size = (vec2_t){size[0].valuedouble, size[1].valuedouble};
        button_layout->datas[i].sprite_idx = sprite_idx->valueint;
        button_layout->datas[i].color = C2D_Color32f(color[0].valuedouble, 
        color[1].valuedouble, color[2].valuedouble, color[3].valuedouble);
        button_layout->datas[i].callback = (button_callback_type_e)callback->valueint;

        i++;
    }

    cJSON_Delete(json);
}
