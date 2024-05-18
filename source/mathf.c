#include "mathf.h"

float Mathf_Lerp(float a, float b, float t) {
    return a + (b - a) / t;
}