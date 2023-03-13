#include <malloc.h>
#include <stdlib.h>

#include "vec_f32.h"

vec_f32 vec_f32_new(u64 len) {
    vec_f32 vec = {
        .data = (f32 *)malloc(len),
        .len = len,
    };
    return vec;
}

void vec_f32_free(vec_f32 *vec) { free(vec->data); }

vec_f32 vec_f32_sum(const vec_f32 *vec1, const vec_f32 *vec2);

vec_f32 vec_f32_mult(const vec_f32 *vec1, const vec_f32 *vec2);

i32 _comp(const void *el1, const void *el2) {
    const int f = *(f32 *)el1;
    const int s = *(f32 *)el2;
    if (f > s)
        return 1;
    if (f < s)
        return -1;
    return 0;
}

void vec_f32_sort(vec_f32 *vec) {
    qsort(vec->data, vec->len, sizeof(f32), _comp);
}
