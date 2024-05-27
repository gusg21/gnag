#ifndef IMAGE_H
#define IMAGE_H

#include "imagedata.h"

struct image;

typedef struct image_s {
    bool initialized;

    bool render_on_top;
    
    image_data_t data;
    vec2_t pos;
    vec2_t size;
    C2D_Sprite sprite;
    u32 color;

    void (*updater)(struct image_s* image);
    
} image_t;

void Image_Init(image_t* image, image_data_t data, C2D_SpriteSheet sheet);
void Image_Update(image_t* image);
void Image_Draw(image_t* image);

#endif