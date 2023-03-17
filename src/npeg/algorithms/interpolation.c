#include <math.h>

#include "../math/matrix.h"
#include "types.h"

static void matrix_u16_bilinear_inter(const Matrix *src, Matrix *dst) {
    u16 *src_data = src->data;
    u16 *dst_data = dst->data;
}

static void matrix_u8_bilinear_inter(const Matrix *src, Matrix *dst) {
    u8 *src_data = (u8 *)src->data;
    u8 *dst_data = (u8 *)dst->data;

    i64 src_height = src->height;
    i64 src_width = src->width;

    for (i64 i = 1; i < src_height - 2; i++) {
        f32 y = (f32)i;
        f32 y1 = y + 1.0f;
        f32 y2 = y - 1.0f;

        f32 y2_minus_y1_inv = 1 / (y2 - y1);
        f32 y2_minus_y = y2 - y;
        f32 y_minus_y1 = y - y1;

        i64 vertical_dst_offset = i * src_width;

        i64 src_vertical_offset_1 = src_width * (i + 1);
        i64 src_vertical_offset_2 = src_width * (i - 1);

        for (i64 j = 1; j < src_width - 2; j++) {
            f32 x = (f32)j;
            f32 x1 = x - 1.0f;
            f32 x2 = x + 1.0f;

            f32 Q11 = (f32)src_data[src_vertical_offset_1 + j - 1];
            f32 Q12 = (f32)src_data[src_vertical_offset_2 + j - 1];
            f32 Q21 = (f32)src_data[src_vertical_offset_1 + j + 1];
            f32 Q22 = (f32)src_data[src_vertical_offset_2 + j + 1];

            f32 x2_minus_x1 = x2 - x1;
            f32 x2_minus_x = x2 - x;
            f32 x_minus_x1 = x - x1;

            f32 first_factor = x2_minus_x / x2_minus_x1;
            f32 second_factor = x_minus_x1 / x2_minus_x1;

            f32 f_x_y1 = first_factor * Q11 + second_factor * Q21;
            f32 f_x_y2 = first_factor * Q12 + second_factor * Q22;

            f32 f_x_y = roundf(y2_minus_y * y2_minus_y1_inv * f_x_y1 +
                               y_minus_y1 * y2_minus_y1_inv * f_x_y2);
            dst_data[vertical_dst_offset + j] = (u8)f_x_y;
        }
    }
}

void matrix_bilinear_inter(Matrix *src) {
    Matrix dst = matrix_new(src->width, src->height, src->matrix_type, true);

    if (src->matrix_type == U8_MATRIX) {
        matrix_u8_bilinear_inter(src, &dst);
    } else {
        matrix_u16_bilinear_inter(src, &dst);
    }

    matrix_free(src);

    *src = dst;
}
