#include "image.h"

#include <citro2d.h>

#include "_defs.h"
#include "uiscripts.h"

void Image_Init(image_t* image, image_data_t data, C2D_SpriteSheet sheet) {
    image->data = data;
    image->pos = data.pos;
    image->size = data.size;
    C2D_SpriteFromSheet(&image->sprite, sheet, data.sprite_idx);
    image->color = data.color;
    image->render_on_top = data.render_on_top;
    image->updater = UIScripts_GetImageUpdaterByType(data.updater_type);
    image->initialized = true;
}

void Image_Update(image_t* image) {
    // Call the updater
    if (image->updater != NULL) {
        image->updater(image);
    }
}

void Image_Draw(image_t* image) {
    if (image->render_on_top) {
        C2D_SpriteSetDepth(&image->sprite, 0.1f);
    }
    C2D_DrawRectSolid(image->pos.x, image->pos.y, 0, image->size.x, image->size.y, image->color);
    C2D_SpriteSetCenter(&image->sprite, .5f, .5f);
    C2D_SpriteSetPos(&image->sprite, image->pos.x, image->pos.y);
    C2D_DrawSprite(&image->sprite);
}