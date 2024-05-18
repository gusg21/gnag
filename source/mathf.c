#include "mathf.h"

float Mathf_Lerp(float a, float b, float t) { return a + (b - a) * t; }

float Mathf_Clamp(float x, float min, float max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}
