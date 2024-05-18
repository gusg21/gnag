#include "vec2.h"

vec2_t Vec2_Add(vec2_t a, vec2_t b) { return (vec2_t){a.x + b.x, a.y + b.y}; }

vec2_t Vec2_Lerp(vec2_t a, vec2_t b, float t) { return (vec2_t){Mathf_Lerp(a.x, b.x, t), Mathf_Lerp(a.y, b.y, t)}; }