#ifndef MATHF_H
#define MATHF_H

#ifdef __cplusplus
extern "C" {
#endif

float Mathf_Lerp(float a, float b, float t);
float Mathf_Clamp(float x, float min, float max);
float Mathf_Min(float a, float b);
float Mathf_Max(float a, float b);

#ifdef __cplusplus
}
#endif

#endif // MATHF_H