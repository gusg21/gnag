#ifndef TEXTDATA_H
#define TEXTDATA_H

#define TEXT_DATA_MAX_TEXT_LENGTH 256

#include "vec2.h"

typedef enum {
    TEXT_UPDATER_NONE,
    TEXT_UPDATER_SELECTED_CHARACTER_NAME
} text_updater_type_e;

typedef struct text_data_s {
    bool initialized;
    char initial_text[TEXT_DATA_MAX_TEXT_LENGTH];
    vec2_t pos;
    bool centered;
    text_updater_type_e updater_type;
} text_data_t;

#endif // TEXTDATA_H