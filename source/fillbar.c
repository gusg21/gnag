#include "fillbar.h"

#include <citro2d.h>

void FillBar_Init(fill_bar_t* bar, fill_bar_data_t data) {
    bar->data = data;
    bar->fill_value = data.max_value;
}

void FillBar_Update(fill_bar_t* bar, float value) {
    bar->fill_value = value;
}

void FillBar_Draw(fill_bar_t* bar) {

}
