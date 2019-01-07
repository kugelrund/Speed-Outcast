#ifndef STRAFE_HELPER_INCLUDES_H
#define STRAFE_HELPER_INCLUDES_H

#ifdef __cplusplus
extern "C" {
#endif

typedef float vec_t;
typedef vec_t vec4_t[4];


static const vec4_t shi_color_accelerating = { 0.0f, 0.5f, 0.125f, 0.375f };
static const vec4_t shi_color_optimal = { 0.0f, 1.0f, 0.25f, 0.75f };
static const vec4_t shi_color_center_marker = { 1.0f, 1.0f,  1.0f, 0.75f };

void shi_drawFilledRectangle(float x, float y, float w, float h,
                             const vec4_t color);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // STRAFE_HELPER_INCLUDES_H
