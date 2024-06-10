#include "mathf.h"

#ifdef __cplusplus
extern "C" {
#endif

float Mathf_Lerp(float a, float b, float t) { return a + (b - a) * t; }

float Mathf_Clamp(float x, float min, float max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

float Mathf_Min(float a, float b) { return (a < b) ? a : b; }

float Mathf_Max(float a, float b) { return (a > b) ? a : b; }

#ifdef __cplusplus
}
#endif
