#ifndef TEXT_H
#define TEXT_H

#include <citro2d.h>

#include "textdata.h"
#include "vec2.h"

typedef struct text_s {
    bool initialized;
    
    C2D_TextBuf buffer;
    C2D_Text text;

    void (*updater)(struct text_s* text);
    vec2_t pos;
} text_t;

void Text_Init(text_t* text, text_data_t data);
void Text_Destroy(text_t* text);
void Text_Update(text_t* text);
void Text_Draw(text_t* text);
void Text_SetText(text_t* text, const char* characters);


#endif // TEXT_H