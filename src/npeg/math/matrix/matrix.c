#include <malloc.h>
#include <math.h>
#include <time.h>

#include "die.h"
#include "matrix.h"

Matrix matrix_new(i64 width, i64 height, MatrixType matrix_type,
                  bool fill_zeroes) {
    Matrix matrix = {
        .height = height,
        .width = width,
    };

    switch (matrix_type) {
    case U8_MATRIX: {
        if (fill_zeroes) {
            matrix.data = (u8 *)calloc((u64)(width * height), sizeof(u8));
        } else {
            matrix.data = (u8 *)malloc((u64)(width * height) * sizeof(u8));
        }
        break;
    }
    case U16_MATRIX: {
        if (fill_zeroes) {
            matrix.data = (u16 *)calloc((u64)(width * height), sizeof(u16));
        } else {
            matrix.data = (u16 *)malloc((u64)(width * height) * sizeof(u16));
        }
        break;
    }
    default:
        die("Error: unknow matrix format. Aviable: U8_MATRIX, U16_MATRIX "
            "only.\n",
            NULL)
    }

    return matrix;
}

void matrix_free(Matrix *matrix) { free(matrix->data); }

static void matrix_upscale_u16(const Matrix *src, Matrix *dst, i64 factor) {
    u16 *dst_data = (u16 *)dst->data;
    u16 *src_data = (u16 *)src->data;

    i64 src_height = src->height;
    i64 src_width = src->width;
    i64 dst_width = src->width;

    for (i64 i = 0; i < src_height; i++) {
        i64 factor_mult_i = factor * i;
        i64 raws_src_offset = i * src_width;

        for (i64 j = 0; j < src_width; j++) {
            i64 factor_mult_j = factor * j;
            i64 src_offset = raws_src_offset + j;

            for (i64 k = 0; k < factor; k++) {
                i64 raws_dst_offset = (factor_mult_i + k) * dst_width;
                for (i64 p = 0; p < factor; p++) {
                    dst_data[raws_dst_offset + (factor_mult_j + p)] =
                        src_data[src_offset];
                }
            }
        }
    }
}

static void matrix_upscale_u8(const Matrix *src, Matrix *dst, i64 factor) {
    u8 *dst_data = (u8 *)dst->data;
    u8 *src_data = (u8 *)src->data;

    i64 src_height = src->height;
    i64 src_width = src->width;
    i64 dst_width = src->width;

    for (i64 i = 0; i < src_height; i++) {
        i64 factor_mult_i = factor * i;
        i64 raws_src_offset = i * src_width;

        for (i64 j = 0; j < src_width; j++) {
            i64 factor_mult_j = factor * j;
            i64 src_offset = raws_src_offset + j;

            for (i64 k = 0; k < factor; k++) {
                i64 raws_dst_offset = (factor_mult_i + k) * dst_width;
                for (i64 p = 0; p < factor; p++) {
                    dst_data[raws_dst_offset + (factor_mult_j + p)] =
                        src_data[src_offset];
                }
            }
        }
    }
}

void matrix_upscale(Matrix *src, i64 factor) {
    Matrix dst = matrix_new(src->width * factor, src->height * factor,
                            src->matrix_type, false);

    if (src->matrix_type == U8_MATRIX) {
        matrix_upscale_u8(src, &dst, factor);
    } else {
        matrix_upscale_u16(src, &dst, factor);
    }

    matrix_free(src);
    *src = dst;
}

static void matrix_downscale_u8(const Matrix *src, Matrix *dst, i64 factor) {
    u8 *dst_data = (u8 *)dst->data;
    u8 *src_data = (u8 *)src->data;

    f32 factor_pow = (f32)(factor * factor);

    i64 dst_width = dst->width;
    i64 dst_height = dst->height;
    i64 src_width = src->width;

    // TODO: move loop to 'from dst_height - 1 to 0'
    for (i64 i = 0; i < dst_height; i++) {
        i64 factor_mult_i = factor * i;
        i64 dst_width_mul_i = dst_width * i;

        for (i64 j = 0; j < dst_width; j++) {
            i64 dst_offset = dst_width_mul_i + j;
            i64 factor_mult_j = factor * j;
            i64 sum = 0;

            for (i64 k = 0; k < factor; k++) {
                i64 vertical_offset = (factor_mult_i + k) * src_width;

                for (i64 p = 0; p < factor; p++) {
                    sum += src_data[vertical_offset + (factor_mult_j + p)];
                }
            }
            dst_data[dst_offset] = (u8)roundf((f32)sum / factor_pow);
        }
    }
}

static void matrix_downscale_u16(const Matrix *src, Matrix *dst, i64 factor) {
    u16 *dst_data = (u16 *)dst->data;
    u16 *src_data = (u16 *)src->data;

    for (i64 i = 0; i < dst->height; i++) {
        for (i64 j = 0; j < dst->width; j++) {
            i64 sum = 0;
            for (i64 k = 0; k < factor; k++) {
                for (i64 p = 0; p < factor; p++) {
                    sum += src_data[(factor * i + k) * src->width +
                                    (factor * j + p)];
                }
            }
            dst_data[i * dst->width + j] =
                (u16)roundf((f32)sum / (f32)(factor * factor));
        }
    }
}

void matrix_downscale(Matrix *src, i64 factor) {
    Matrix dst =
        matrix_new(src->width / factor, src->height / factor, U8_MATRIX, false);

    if (src->matrix_type == U8_MATRIX) {
        matrix_downscale_u8(src, &dst, factor);
    } else {
        matrix_downscale_u16(src, &dst, factor);
    }

    matrix_free(src);
    *src = dst;
}
