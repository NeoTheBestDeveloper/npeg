#include <malloc.h>
#include <math.h>

#include "matrix_u8.h"

#define PI 3.141592653589793f

static f32 deg_to_rad(f32 degrees) { return degrees * (PI / 180.0f); }

matrix_u8 matrix_u8_new(u64 width, u64 height) {
    matrix_u8 matrix = {
        .height = height,
        .width = width,
        .data = (u8 *)malloc(width * height),
    };

    return matrix;
}

void matrix_u8_free(matrix_u8 *matrix) { free(matrix->data); }

void matrix_u8_rotate(matrix_u8 *src, f32 degrees) {
    matrix_u8 new_matrix = matrix_u8_new(src->width, src->height);

    f32 cos_a = cosf(deg_to_rad(degrees));
    f32 sin_a = sinf(deg_to_rad(degrees));

    for (u64 i = 0; i < src->height; i++) {
        for (u64 j = 0; j < src->height; j++) {
            u8 new_x = (u8)((f32)j * cos_a - (f32)i * sin_a);
            u8 new_y = (u8)((f32)i * cos_a + (f32)j * sin_a);

            new_matrix.data[new_y * src->height + new_x] =
                src->data[i * src->height + j];
        }
    }

    matrix_u8_free(src);
    *src = new_matrix;
}
