#ifndef H_VEC_F32
#define H_VEC_F32

#include "types.h"

typedef struct {
    f32 *data;
    u64 len;
} vec_f32;

vec_f32 vec_f32_new(u64 len);
void vec_f32_free(vec_f32 *vec);

vec_f32 vec_f32_sum(const vec_f32 *vec1, const vec_f32 *vec2);
vec_f32 vec_f32_mult(const vec_f32 *vec1, const vec_f32 *vec2);
void vec_f32_sort(vec_f32 *vec);

#endif // !H_VEC_F32
