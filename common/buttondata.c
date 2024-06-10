#include "buttondata.h"

#ifdef __cplusplus
extern "C" {
#endif

colorf_t ButtonData_GetPressedColor(colorf_t base_color) {
    return (colorf_t){Mathf_Lerp(base_color.r, 0.f, .5f), Mathf_Lerp(base_color.g, 0.f, .5f),
                      Mathf_Lerp(base_color.b, 0.f, .5f), base_color.a};
}

#ifdef __cplusplus
}
#endif