#include "uilayout.h"

#include <stdlib.h>
#include <string.h>

#include "_defs.h"
#include "colorf.h"
#include "jsonhelper.h"

#define UILAYOUT_MAX_FILE_SIZE 65536

#ifdef __cplusplus
extern "C" {
#endif

void UILayout_InitFromFile(ui_layout_t* layout, const char* filename) {
    memset(layout->button_datas, 0, sizeof(button_data_t) * UILAYOUT_MAX_BUTTON_COUNT);
    memset(layout->fill_bar_datas, 0, sizeof(fill_bar_data_t) * UILAYOUT_MAX_FILL_BAR_COUNT);
    memset(layout->text_datas, 0, sizeof(text_data_t) * UILAYOUT_MAX_TEXT_COUNT);
    memset(layout->image_datas, 0, sizeof(image_data_t) * UILAYOUT_MAX_IMAGE_COUNT);

    UILayout_LoadNewLayoutFromFile(layout, filename);
}

void UILayout_LoadNewLayoutFromFile(ui_layout_t* layout, const char* filename) {
    cJSON* json = JSONHelper_LoadCJSONFromFile(filename, UILAYOUT_MAX_FILE_SIZE);

    UILayout_LoadButtonsFromJSON(layout, json);
    UILayout_LoadFillBarsFromJSON(layout, json);
    UILayout_LoadTextsFromJSON(layout, json);
    UILayout_LoadImagesFromJSON(layout, json);

    cJSON_Delete(json);
}

void UILayout_LoadButtonsFromJSON(ui_layout_t* layout, cJSON* json) {
    cJSON* buttons = cJSON_GetObjectItem(json, "Buttons");
    cJSON* button;
    u32 button_index = 0;

    cJSON_ArrayForEach(button, buttons) {
        // Get & Set Position
        cJSON* pos = cJSON_GetObjectItem(button, "Pos");
        cJSON* x = cJSON_GetArrayItem(pos, 0);
        cJSON* y = cJSON_GetArrayItem(pos, 1);
        layout->button_datas[button_index].pos = (vec2_t){(float)x->valuedouble, (float)y->valuedouble};

        // Get & Set Size
        cJSON* size = cJSON_GetObjectItem(button, "Size");
        x = cJSON_GetArrayItem(size, 0);
        y = cJSON_GetArrayItem(size, 1);
        layout->button_datas[button_index].size = (vec2_t){(float)x->valuedouble, (float)y->valuedouble};

        // Get & Set Sprite Index
        cJSON* sprite_idx = cJSON_GetObjectItem(button, "Sprite Index");
        if (sprite_idx->valueint != -1) {
            layout->button_datas[button_index].sprite_idx = sprite_idx->valueint;
        }

        // Get & Set Color
        cJSON* color = cJSON_GetObjectItem(button, "Color");
        cJSON* r = cJSON_GetArrayItem(color, 0);
        cJSON* g = cJSON_GetArrayItem(color, 1);
        cJSON* b = cJSON_GetArrayItem(color, 2);
        cJSON* a = cJSON_GetArrayItem(color, 3);
        layout->button_datas[button_index].color =
            (colorf_t){(float)r->valuedouble, (float)g->valuedouble, (float)b->valuedouble, (float)a->valuedouble};

        // Get & Set Callback
        cJSON* callback_type = cJSON_GetObjectItem(button, "Callback");
        if (callback_type != NULL) {  // Having a callback is optional
            layout->button_datas[button_index].callback_type = callback_type->valueint;
        } else {
            layout->button_datas[button_index].callback_type = BUTTON_CALLBACK_NONE;
        }

        // Get & Set Updater
        cJSON* updater_type = cJSON_GetObjectItem(button, "Updater");
        if (updater_type != NULL) {  // optional
            layout->button_datas[button_index].updater_type = updater_type->valueint;
        } else {
            layout->button_datas[button_index].updater_type = BUTTON_UPDATER_NONE;
        }

        // Mark Button Data as initialized
        layout->button_datas[button_index].initialized = true;

        // Simple Indexing
        button_index++;
    }
}

void UILayout_LoadFillBarsFromJSON(ui_layout_t* layout, cJSON* json) {
    cJSON* fill_bars = cJSON_GetObjectItem(json, "Fillbars");
    cJSON* fill_bar;
    u32 fill_bar_index = 0;

    cJSON_ArrayForEach(fill_bar, fill_bars) {
        // Get & Set Position
        cJSON* pos = cJSON_GetObjectItem(fill_bar, "Pos");
        cJSON* x = cJSON_GetArrayItem(pos, 0);
        cJSON* y = cJSON_GetArrayItem(pos, 1);
        layout->fill_bar_datas[fill_bar_index].pos = (vec2_t){(float)x->valuedouble, (float)y->valuedouble};

        // Get & Set Size
        cJSON* size = cJSON_GetObjectItem(fill_bar, "Size");
        x = cJSON_GetArrayItem(size, 0);
        y = cJSON_GetArrayItem(size, 1);
        layout->fill_bar_datas[fill_bar_index].size = (vec2_t){(float)x->valuedouble, (float)y->valuedouble};

        // Get & Set Color
        cJSON* color = cJSON_GetObjectItem(fill_bar, "Background Color");
        cJSON* r = cJSON_GetArrayItem(color, 0);
        cJSON* g = cJSON_GetArrayItem(color, 1);
        cJSON* b = cJSON_GetArrayItem(color, 2);
        cJSON* a = cJSON_GetArrayItem(color, 3);
        layout->fill_bar_datas[fill_bar_index].bg_color =
            (colorf_t){(float)r->valuedouble, (float)g->valuedouble, (float)b->valuedouble, (float)a->valuedouble};

        // Get & Set Color
        color = cJSON_GetObjectItem(fill_bar, "Fill Color");
        r = cJSON_GetArrayItem(color, 0);
        g = cJSON_GetArrayItem(color, 1);
        b = cJSON_GetArrayItem(color, 2);
        a = cJSON_GetArrayItem(color, 3);
        layout->fill_bar_datas[fill_bar_index].fill_color =
            (colorf_t){(float)r->valuedouble, (float)g->valuedouble, (float)b->valuedouble, (float)a->valuedouble};

        // Get & Set Updater
        cJSON* updater_type = cJSON_GetObjectItem(fill_bar, "Updater");
        if (updater_type != NULL) {  // optional
            layout->fill_bar_datas[fill_bar_index].updater_type = updater_type->valueint;
        } else {
            layout->fill_bar_datas[fill_bar_index].updater_type = FILL_BAR_UPDATER_NONE;
        }

        // Mark Fill Bar Data as initialized
        layout->fill_bar_datas[fill_bar_index].initialized = true;

        fill_bar_index++;
    }
}

void UILayout_LoadImagesFromJSON(ui_layout_t* layout, cJSON* json) {
    cJSON* images = cJSON_GetObjectItem(json, "Images");
    cJSON* image;
    u32 image_index = 0;

    cJSON_ArrayForEach(image, images) {
        // Get & Set Position
        cJSON* pos = cJSON_GetObjectItem(image, "Pos");
        cJSON* x = cJSON_GetArrayItem(pos, 0);
        cJSON* y = cJSON_GetArrayItem(pos, 1);
        layout->image_datas[image_index].pos = (vec2_t){(float)x->valuedouble, (float)y->valuedouble};

        // Get & Set Size
        cJSON* size = cJSON_GetObjectItem(image, "Size");
        x = cJSON_GetArrayItem(size, 0);
        y = cJSON_GetArrayItem(size, 1);
        layout->image_datas[image_index].size = (vec2_t){(float)x->valuedouble, (float)y->valuedouble};

        // Get & Set Sprite Index
        cJSON* sprite_idx = cJSON_GetObjectItem(image, "Sprite Index");
        if (sprite_idx->valueint != -1) {
            layout->image_datas[image_index].sprite_idx = sprite_idx->valueint;
        }

        // Get & Set Color
        cJSON* color = cJSON_GetObjectItem(image, "Color");
        cJSON* r = cJSON_GetArrayItem(color, 0);
        cJSON* g = cJSON_GetArrayItem(color, 1);
        cJSON* b = cJSON_GetArrayItem(color, 2);
        cJSON* a = cJSON_GetArrayItem(color, 3);
        layout->image_datas[image_index].color = (colorf_t){(float)r->valuedouble, (float)g->valuedouble, (float)b->valuedouble, (float)a->valuedouble};

        cJSON* top = cJSON_GetObjectItem(image, "Render On Top");
        if (cJSON_IsBool(top)) {
            if (cJSON_IsTrue(top))
                layout->image_datas[image_index].render_on_top = true;
            else
                layout->image_datas[image_index].render_on_top = false;
        }

        // Get & Set Updater
        cJSON* updater_type = cJSON_GetObjectItem(image, "Updater");
        if (updater_type != NULL) {  // optional
            layout->image_datas[image_index].updater_type = updater_type->valueint;
        } else {
            layout->image_datas[image_index].updater_type = IMAGE_UPDATER_NONE;
        }

        // Mark Image Data as initialized
        layout->image_datas[image_index].initialized = true;

        image_index++;
    }
}

void UILayout_LoadTextsFromJSON(ui_layout_t* layout, cJSON* json) {
    cJSON* texts = cJSON_GetObjectItem(json, "Texts");
    cJSON* text;
    u32 text_index = 0;

    cJSON_ArrayForEach(text, texts) {
        // Get initial text
        cJSON* initial_text_object = cJSON_GetObjectItem(text, "Initial Text");
        size_t initial_text_length = strlen(initial_text_object->valuestring);
        memcpy(layout->text_datas[text_index].initial_text, initial_text_object->valuestring, initial_text_length);

        // Get & Set Position
        cJSON* pos = cJSON_GetObjectItem(text, "Pos");
        cJSON* x = cJSON_GetArrayItem(pos, 0);
        cJSON* y = cJSON_GetArrayItem(pos, 1);
        layout->text_datas[text_index].pos = (vec2_t){(float)x->valuedouble, (float)y->valuedouble};

        // Get & Set Alignment
        cJSON* centered = cJSON_GetObjectItem(text, "Centered");
        if (cJSON_IsBool(centered)) {
            if (cJSON_IsTrue(centered))
                layout->text_datas[text_index].centered = true;
            else
                layout->text_datas[text_index].centered = false;
        }

        // Get Updater type
        cJSON* updater_type = cJSON_GetObjectItem(text, "Updater");
        if (updater_type != NULL) {  // optional
            layout->text_datas[text_index].updater_type = updater_type->valueint;
        } else {
            layout->text_datas[text_index].updater_type = TEXT_UPDATER_NONE;
        }

        // Mark Fill Bar Data as initialized
        layout->text_datas[text_index].initialized = true;

        text_index++;
    }
}

void UILayout_SaveToFile(ui_layout_t *layout, const char *filename) {

}

#ifdef __cplusplus
}
#endif