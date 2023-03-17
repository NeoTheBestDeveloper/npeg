#include <math.h>

#include "../math/matrix/matrix.h"
#include "../math/trig.h"
#include "algorithms.h"
#include "types.h"

static void matrix_u8_rotate(const Matrix *src, Matrix *dst, f32 sin_a,
                             f32 cos_a) {
    u8 *src_data = (u8 *)src->data;
    u8 *dst_data = (u8 *)dst->data;

    f32 height_half = src->height >> 1;
    f32 width_half = src->width >> 1;

    for (i64 i = src->height - 1; i != 0; --i) {
        f32 old_y = height_half - i;
        i64 raws_offset = i * src->width;

        for (i64 j = src->width - 1; j != 0; --j) {
            f32 old_x = width_half - j;

            f32 new_x = old_x * cos_a - old_y * sin_a;
            f32 new_y = old_y * cos_a + old_x * sin_a;

            u64 new_j = (u64)((f32)width_half - new_x);
            u64 new_i = (u64)((f32)height_half - new_y);

            if (new_i < src->height && new_j < src->width) {
                dst_data[new_i * src->width + new_j] =
                    src_data[raws_offset + j];
            }
        }
    }
}

static void matrix_u16_rotate(const Matrix *src, Matrix *dst, f32 sin_a,
                              f32 cos_a) {
    u16 *src_data = (u16 *)src->data;
    u16 *dst_data = (u16 *)dst->data;

    for (i64 i = 0; i < src->height; i++) {
        for (i64 j = 0; j < src->width; j++) {
            i64 old_x = src->width / 2 - j;
            i64 old_y = src->height / 2 - i;

            f32 new_x = (f32)old_x * cos_a - (f32)old_y * sin_a;
            f32 new_y = (f32)old_y * cos_a + (f32)old_x * sin_a;

            f32 new_j = (f32)src->width / 2.0f - new_x;
            f32 new_i = (f32)src->height / 2.0f - new_y;

            if ((i64)new_i < src->height && (i64)new_j < src->width &&
                new_i > 0 && new_j > 0) {
                dst_data[(i64)new_i * src->width + (i64)new_j] =
                    src_data[i * src->width + j];
            }
        }
    }
}

void matrix_rotate(Matrix *src, f32 degrees) {
    Matrix dst =
        matrix_new(src->width * 2, src->height * 2, src->matrix_type, true);
    matrix_upscale(src, 2);

    f32 cos_a = cosf(deg_to_rad(degrees));
    f32 sin_a = sinf(deg_to_rad(degrees));

    if (src->matrix_type == U8_MATRIX) {
        matrix_u8_rotate(src, &dst, sin_a, cos_a);
    } else {
        matrix_u16_rotate(src, &dst, sin_a, cos_a);
    }

    matrix_free(src);
    matrix_downscale(&dst, 2);

    *src = dst;
}
