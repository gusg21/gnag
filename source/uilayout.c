#include "uilayout.h"

#include <citro2d.h>
#include "stdlib.h"

#include "_defs.h"
#include "panic.h"
#include "uisprites.h"

#define UILAYOUT_MAX_FILE_SIZE 8192

void UILayout_InitFromFile(ui_layout_t* layout, const char* filename) {
    memset(layout->button_datas, 0, sizeof(button_data_t) * UILAYOUT_MAX_BUTTON_COUNT);
    memset(layout->fill_bar_datas, 0, sizeof(fill_bar_data_t) * UILAYOUT_MAX_FILL_BAR_COUNT);
    memset(layout->text_datas, 0, sizeof(text_data_t) * UILAYOUT_MAX_TEXT_COUNT);

    UILayout_LoadNewLayoutFromFile(layout, filename);
}

void UILayout_LoadNewLayoutFromFile(ui_layout_t* layout, const char* filename) {
    FILE* fptr = fopen(filename, "r");
    if (fptr == NULL) {
        Panic_Panic();
        CTR_PRINTF("Unable to open file %s\n", filename);
    }

    char* buffer = malloc(sizeof(char) * UILAYOUT_MAX_FILE_SIZE);
    fread(buffer, sizeof(char), UILAYOUT_MAX_FILE_SIZE, fptr);
    fclose(fptr);

    cJSON* json = cJSON_Parse(buffer);
    if (json == NULL) {
        Panic_Panic();
        size_t error_byte_offset = (size_t)cJSON_GetErrorPtr() - (size_t)buffer;
        char* begin = &buffer[MAX(error_byte_offset - 50, 0)];
        size_t length = 50;
        CTR_PRINTF("Error loading JSON from %s\n", filename);
        CTR_PRINTF("%*.*s\n", length, length, begin);
        CTR_PRINTF("%d bytes from start of file\n", error_byte_offset);
    }

    UILayout_LoadButtonsFromJSON(layout, json);
    UILayout_LoadFillBarsFromJSON(layout, json);
    UILayout_LoadTextsFromJSON(layout, json);
    UILayout_LoadImagesFromJSON(layout, json);

    cJSON_Delete(json);
    free(buffer);
}

void UILayout_LoadButtonsFromJSON(ui_layout_t* layout, cJSON* json) {
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
        if (sprite_idx->valueint != -1) {
            layout->button_datas[index].sprite_idx = sprite_idx->valueint;
        }

        // Get & Set Color
        cJSON* color = cJSON_GetObjectItem(button, "Color");
        cJSON* r = cJSON_GetArrayItem(color, 0);
        cJSON* g = cJSON_GetArrayItem(color, 1);
        cJSON* b = cJSON_GetArrayItem(color, 2);
        cJSON* a = cJSON_GetArrayItem(color, 3);
        layout->button_datas[index].color =
            C2D_Color32f(r->valuedouble, g->valuedouble, b->valuedouble, a->valuedouble);
        layout->button_datas[index].pressed_color =
            C2D_Color32f(Mathf_Lerp(r->valuedouble, .5f, .5f), Mathf_Lerp(g->valuedouble, .5f, .5f),
                         Mathf_Lerp(b->valuedouble, .5f, .5f), a->valuedouble);

        // Get & Set Callback
        cJSON* callback_type = cJSON_GetObjectItem(button, "Callback");
        if (callback_type != NULL) {  // Having a callback is optional
            layout->button_datas[index].callback_type = callback_type->valueint;
        } else {
            layout->button_datas[index].callback_type = BUTTON_CALLBACK_NONE;
        }

        // Get & Set Updater
        cJSON* updater_type = cJSON_GetObjectItem(button, "Updater");
        if (updater_type != NULL) {  // optional
            layout->button_datas[index].updater_type = updater_type->valueint;
        } else {
            layout->button_datas[index].updater_type = BUTTON_UPDATER_NONE;
        }

        // Mark Button Data as initialized
        layout->button_datas[index].initialized = true;
    }
}

void UILayout_LoadFillBarsFromJSON(ui_layout_t* layout, cJSON* json) {
    cJSON* fill_bars = cJSON_GetObjectItem(json, "Fillbars");
    cJSON* fill_bar;

    cJSON_ArrayForEach(fill_bar, fill_bars) {
        // Get index
        cJSON* index_object = cJSON_GetObjectItem(fill_bar, "Index");
        u32 index = index_object->valueint;

        // Get & Set Position
        cJSON* pos = cJSON_GetObjectItem(fill_bar, "Pos");
        cJSON* x = cJSON_GetArrayItem(pos, 0);
        cJSON* y = cJSON_GetArrayItem(pos, 1);
        layout->fill_bar_datas[index].pos = (vec2_t){x->valuedouble, y->valuedouble};

        // Get & Set Size
        cJSON* size = cJSON_GetObjectItem(fill_bar, "Size");
        x = cJSON_GetArrayItem(size, 0);
        y = cJSON_GetArrayItem(size, 1);
        layout->fill_bar_datas[index].size = (vec2_t){x->valuedouble, y->valuedouble};

        // Get & Set Color
        cJSON* color = cJSON_GetObjectItem(fill_bar, "Background Color");
        cJSON* r = cJSON_GetArrayItem(color, 0);
        cJSON* g = cJSON_GetArrayItem(color, 1);
        cJSON* b = cJSON_GetArrayItem(color, 2);
        cJSON* a = cJSON_GetArrayItem(color, 3);
        layout->fill_bar_datas[index].bg_color =
            C2D_Color32f(r->valuedouble, g->valuedouble, b->valuedouble, a->valuedouble);

        // Get & Set Color
        color = cJSON_GetObjectItem(fill_bar, "Fill Color");
        r = cJSON_GetArrayItem(color, 0);
        g = cJSON_GetArrayItem(color, 1);
        b = cJSON_GetArrayItem(color, 2);
        a = cJSON_GetArrayItem(color, 3);
        layout->fill_bar_datas[index].fill_color =
            C2D_Color32f(r->valuedouble, g->valuedouble, b->valuedouble, a->valuedouble);

        // Get & Set Value Max
        cJSON* max_value = cJSON_GetObjectItem(fill_bar, "Max Value");
        layout->fill_bar_datas[index].max_value = max_value->valuedouble;

        // Get & Set Updater
        cJSON* updater_type = cJSON_GetObjectItem(fill_bar, "Updater");
        if (updater_type != NULL) {  // optional
            layout->fill_bar_datas[index].updater_type = updater_type->valueint;
        } else {
            layout->fill_bar_datas[index].updater_type = FILL_BAR_UPDATER_NONE;
        }

        // Mark Fill Bar Data as initialized
        layout->fill_bar_datas[index].initialized = true;
    }
}

void UILayout_LoadImagesFromJSON(ui_layout_t* layout, cJSON* json) {
    cJSON* images = cJSON_GetObjectItem(json, "Images");
    cJSON* image;

    cJSON_ArrayForEach(image, images) {
        // Get index
        cJSON* index_object = cJSON_GetObjectItem(image, "Index");
        u32 index = index_object->valueint;

        // Get & Set Position
        cJSON* pos = cJSON_GetObjectItem(image, "Pos");
        cJSON* x = cJSON_GetArrayItem(pos, 0);
        cJSON* y = cJSON_GetArrayItem(pos, 1);
        layout->image_datas[index].pos = (vec2_t){x->valuedouble, y->valuedouble};

        // Get & Set Size
        cJSON* size = cJSON_GetObjectItem(image, "Size");
        x = cJSON_GetArrayItem(size, 0);
        y = cJSON_GetArrayItem(size, 1);
        layout->image_datas[index].size = (vec2_t){x->valuedouble, y->valuedouble};

        // Get & Set Sprite Index
        cJSON* sprite_idx = cJSON_GetObjectItem(image, "Sprite Index");
        if (sprite_idx->valueint != -1) {
            layout->image_datas[index].sprite_idx = sprite_idx->valueint;
        }

        // Get & Set Color
        cJSON* color = cJSON_GetObjectItem(image, "Color");
        cJSON* r = cJSON_GetArrayItem(color, 0);
        cJSON* g = cJSON_GetArrayItem(color, 1);
        cJSON* b = cJSON_GetArrayItem(color, 2);
        cJSON* a = cJSON_GetArrayItem(color, 3);
        layout->image_datas[index].color = C2D_Color32f(r->valuedouble, g->valuedouble, b->valuedouble, a->valuedouble);

        cJSON* top = cJSON_GetObjectItem(image, "Render On Top");
        if (cJSON_IsBool(top)) {
            if (cJSON_IsTrue(top))
                layout->image_datas[index].render_on_top = true;
            else
                layout->image_datas[index].render_on_top = false;
        }

        // Get & Set Updater
        cJSON* updater_type = cJSON_GetObjectItem(image, "Updater");
        if (updater_type != NULL) {  // optional
            layout->image_datas[index].updater_type = updater_type->valueint;
        } else {
            layout->image_datas[index].updater_type = IMAGE_UPDATER_NONE;
        }

        // Mark Image Data as initialized
        layout->image_datas[index].initialized = true;
    }
}

void UILayout_LoadTextsFromJSON(ui_layout_t* layout, cJSON* json) {
    cJSON* texts = cJSON_GetObjectItem(json, "Texts");
    cJSON* text;

    cJSON_ArrayForEach(text, texts) {
        // Get index
        cJSON* index_object = cJSON_GetObjectItem(text, "Index");
        u32 index = index_object->valueint;

        // Get initial text
        cJSON* initial_text_object = cJSON_GetObjectItem(text, "Initial Text");
        size_t initial_text_length = strlen(initial_text_object->valuestring);
        memcpy(layout->text_datas[index].initial_text, initial_text_object->valuestring, initial_text_length);

        // Get & Set Position
        cJSON* pos = cJSON_GetObjectItem(text, "Pos");
        cJSON* x = cJSON_GetArrayItem(pos, 0);
        cJSON* y = cJSON_GetArrayItem(pos, 1);
        layout->text_datas[index].pos = (vec2_t){x->valuedouble, y->valuedouble};

        // Get & Set Alignment
        cJSON* centered = cJSON_GetObjectItem(text, "Centered");
        if (cJSON_IsBool(centered)) {
            if (cJSON_IsTrue(centered))
                layout->text_datas[index].centered = true;
            else
                layout->text_datas[index].centered = false;
        }

        // Get Updater type
        cJSON* updater_type = cJSON_GetObjectItem(text, "Updater");
        if (updater_type != NULL) {  // optional
            layout->text_datas[index].updater_type = updater_type->valueint;
            CTR_PRINTF("Text updater type# %d\n", updater_type->valueint);
        } else {
            layout->text_datas[index].updater_type = TEXT_UPDATER_NONE;
            CTR_PRINTF("No updater field for text, loading 0\n");
        }

        // Mark Fill Bar Data as initialized
        layout->text_datas[index].initialized = true;
    }
}
