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
    UILayout_InitEmpty(layout);
    UILayout_LoadNewLayoutFromFile(layout, filename);
}

void UILayout_InitEmpty(ui_layout_t* layout) {
    memset(layout->button_datas, 0, sizeof(button_data_t) * UILAYOUT_MAX_BUTTON_COUNT);
    memset(layout->fill_bar_datas, 0, sizeof(fill_bar_data_t) * UILAYOUT_MAX_FILL_BAR_COUNT);
    memset(layout->text_datas, 0, sizeof(text_data_t) * UILAYOUT_MAX_TEXT_COUNT);
    memset(layout->image_datas, 0, sizeof(image_data_t) * UILAYOUT_MAX_IMAGE_COUNT);
}

void UILayout_LoadNewLayoutFromFile(ui_layout_t* layout, const char* filename) {
    cJSON* json = JSONHelper_LoadCJSONFromFile(filename);

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
        layout->image_datas[image_index].color =
            (colorf_t){(float)r->valuedouble, (float)g->valuedouble, (float)b->valuedouble, (float)a->valuedouble};

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

void UILayout_SaveToFile(ui_layout_t* layout, const char* filename) {
    cJSON* ui_layout_json = cJSON_CreateObject();

    // Texts
    cJSON* texts_json = cJSON_CreateArray();
    for (uint32_t text_index = 0; text_index < UILAYOUT_MAX_TEXT_COUNT; text_index++) {
        if (!layout->text_datas[text_index].initialized) continue;

        text_data_t* text_data = &layout->text_datas[text_index];
        cJSON* text_json = cJSON_CreateObject();

        cJSON_AddItemToObject(text_json, "Initial Text", cJSON_CreateString(text_data->initial_text));

        cJSON_AddItemToObject(text_json, "Pos", cJSON_CreateFloatArray(&text_data->pos.x, 2));

        cJSON_AddItemToObject(text_json, "Centered", cJSON_CreateBool(text_data->centered));

        cJSON_AddItemToObject(text_json, "Updater", cJSON_CreateNumber(text_data->updater_type));

        cJSON_AddItemToArray(texts_json, text_json);
    }
    cJSON_AddItemToObject(ui_layout_json, "Texts", texts_json);

    // Images
    cJSON* images_json = cJSON_CreateArray();
    for (uint32_t image_index = 0; image_index < UILAYOUT_MAX_IMAGE_COUNT; image_index++) {
        if (!layout->image_datas[image_index].initialized) continue;

        image_data_t* image_data = &layout->image_datas[image_index];
        cJSON* image_json = cJSON_CreateObject();

        cJSON_AddItemToObject(image_json, "Pos", cJSON_CreateFloatArray(&image_data->pos.x, 2));

        cJSON_AddItemToObject(image_json, "Size", cJSON_CreateFloatArray(&image_data->size.x, 2));

        cJSON_AddItemToObject(image_json, "Sprite Index", cJSON_CreateNumber(image_data->sprite_idx));

        cJSON_AddItemToObject(image_json, "Color", cJSON_CreateFloatArray(&image_data->color.r, 4));

        cJSON_AddItemToObject(image_json, "Render On Top", cJSON_CreateBool(image_data->render_on_top));

        cJSON_AddItemToObject(image_json, "Updater", cJSON_CreateNumber(image_data->updater_type));

        cJSON_AddItemToArray(images_json, image_json);
    }
    cJSON_AddItemToObject(ui_layout_json, "Images", images_json);

    // Fillbars
    cJSON* fill_bars_json = cJSON_CreateArray();
    for (uint32_t fill_bar_index = 0; fill_bar_index < UILAYOUT_MAX_FILL_BAR_COUNT; fill_bar_index++) {
        if (!layout->fill_bar_datas[fill_bar_index].initialized) continue;

        fill_bar_data_t* fill_bar_data = &layout->fill_bar_datas[fill_bar_index];
        cJSON* fill_bar_json = cJSON_CreateObject();

        cJSON_AddItemToObject(fill_bar_json, "Pos", cJSON_CreateFloatArray(&fill_bar_data->pos.x, 2));

        cJSON_AddItemToObject(fill_bar_json, "Size", cJSON_CreateFloatArray(&fill_bar_data->size.x, 2));

        cJSON_AddItemToObject(fill_bar_json, "Background Color", cJSON_CreateFloatArray(&fill_bar_data->bg_color.r, 4));

        cJSON_AddItemToObject(fill_bar_json, "Fill Color", cJSON_CreateFloatArray(&fill_bar_data->fill_color.r, 4));

        cJSON_AddItemToObject(fill_bar_json, "Updater", cJSON_CreateNumber(fill_bar_data->updater_type));

        cJSON_AddItemToArray(fill_bars_json, fill_bar_json);
    }
    cJSON_AddItemToObject(ui_layout_json, "Fillbars", fill_bars_json);

    // Buttons
    cJSON* buttons_json = cJSON_CreateArray();
    for (uint32_t button_index = 0; button_index < UILAYOUT_MAX_BUTTON_COUNT; button_index++) {
        if (!layout->button_datas[button_index].initialized) continue;

        button_data_t* button_data = &layout->button_datas[button_index];
        cJSON* button_json = cJSON_CreateObject();

        cJSON_AddItemToObject(button_json, "Pos", cJSON_CreateFloatArray(&button_data->pos.x, 2));

        cJSON_AddItemToObject(button_json, "Size", cJSON_CreateFloatArray(&button_data->size.x, 2));

        cJSON_AddItemToObject(button_json, "Sprite Index", cJSON_CreateNumber(button_data->sprite_idx));

        cJSON_AddItemToObject(button_json, "Color", cJSON_CreateFloatArray(&button_data->color.r, 4));

        cJSON_AddItemToObject(button_json, "Callback", cJSON_CreateNumber(button_data->callback_type));

        cJSON_AddItemToObject(button_json, "Updater", cJSON_CreateNumber(button_data->updater_type));

        cJSON_AddItemToArray(buttons_json, button_json);
    }
    cJSON_AddItemToObject(ui_layout_json, "Buttons", buttons_json);

    // Serialize
    JSONHelper_WriteCJSONToFile(ui_layout_json, filename);
}

#ifdef __cplusplus
}
#endif