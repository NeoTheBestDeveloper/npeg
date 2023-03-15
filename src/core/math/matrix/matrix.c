#include <malloc.h>
#include <math.h>
#include <stdio.h>

#include "die.h"
#include "matrix.h"

#define PI 3.141592653589793f
#define EPS 0.5f

static f32 deg_to_rad(f32 degrees) { return degrees * (PI / 180.0f); }

static bool cmp_float(f32 num1, f32 num2) {
    if (num1 > num2) {
        return (num1 - num2) < EPS;
    }
    return (num2 - num1) < EPS;
}

Matrix matrix_new(u64 width, u64 height, MatrixType matrix_type,
                  bool fill_zeroes) {
    Matrix matrix = {
        .height = height,
        .width = width,
    };

    switch (matrix_type) {
    case U8_MATRIX: {

        if (fill_zeroes) {
            matrix.data = (u8 *)calloc(width * height, sizeof(u8));
        } else {
            matrix.data = (u8 *)malloc(width * height * sizeof(u8));
        }
        break;
    }
    case U16_MATRIX: {
        if (fill_zeroes) {
            matrix.data = (u16 *)calloc(width * height, sizeof(u16));
        } else {
            matrix.data = (u16 *)malloc(width * height * sizeof(u16));
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

static void matrix_u8_rotate(Matrix *src, f32 degrees) {
    Matrix dst = matrix_new(src->width, src->height, U8_MATRIX, true);

    f32 cos_a = cosf(deg_to_rad(degrees));
    f32 sin_a = sinf(deg_to_rad(degrees));

    u8 *src_data = (u8 *)src->data;
    u8 *dst_data = (u8 *)dst.data;

    for (u64 i = 0; i < src->height; i++) {
        for (u64 j = 0; j < src->width; j++) {
            i64 old_x = (i64)src->width / 2 - (i64)j;
            i64 old_y = (i64)src->height / 2 - (i64)i;

            f32 new_x = (f32)old_x * cos_a - (f32)old_y * sin_a;
            f32 new_y = (f32)old_y * cos_a + (f32)old_x * sin_a;

            f32 new_j = (f32)src->width / 2.0f - new_x;
            f32 new_i = (f32)src->height / 2.0f - new_y;

            if ((u64)new_i < src->height && (u64)new_j < src->width) {
                dst_data[(u64)new_i * src->width + (u64)new_j] =
                    src_data[i * src->width + j];
            }

            if (cmp_float(new_j - (u32)new_j, 0.5f) &&
                cmp_float(new_i - (u32)new_i, 0.5f)) {
                if ((u64)new_i < src->height && ((u64)new_j + 1) < src->width) {
                    dst_data[(u64)new_i * src->width + (u64)new_j + 1] =
                        src_data[i * src->width + j];
                }

                if (((u64)new_i + 1) < src->height && (u64)new_j < src->width) {
                    dst_data[((u64)new_i + 1) * src->width + (u64)new_j] =
                        src_data[i * src->width + j];
                }

                if (((u64)new_i + 1) < src->height &&
                    ((u64)new_j + 1) < src->width) {
                    dst_data[((u64)new_i + 1) * src->width + (u64)new_j + 1] =
                        src_data[i * src->width + j];
                }
            }

            if (cmp_float(new_j - (u32)new_j, 0.5f)) {
                if ((u64)new_i < src->height && ((u64)new_j + 1) < src->width) {
                    dst_data[(u64)new_i * src->width + (u64)new_j + 1] =
                        src_data[i * src->width + j];
                }
            }

            if (cmp_float(new_i - (u32)new_i, 0.5f)) {
                if (((u64)new_i + 1) < src->height && (u64)new_j < src->width) {
                    dst_data[((u64)new_i + 1) * src->width + (u64)new_j] =
                        src_data[i * src->width + j];
                }
            }
        }
    }

    matrix_free(src);
    *src = dst;
}

static void matrix_u16_rotate(Matrix *src, f32 degrees) {
    Matrix dst = matrix_new(src->width, src->height, U16_MATRIX, true);

    f32 cos_a = cosf(deg_to_rad(degrees));
    f32 sin_a = sinf(deg_to_rad(degrees));

    u16 *src_data = (u16 *)src->data;
    u16 *dst_data = (u16 *)dst.data;

    for (u64 i = 0; i < src->height; i++) {
        for (u64 j = 0; j < src->height; j++) {
            u64 new_x = (u64)((f32)j * cos_a - (f32)i * sin_a);
            u64 new_y = (u64)((f32)i * cos_a + (f32)j * sin_a);

            if (new_y < src->height && new_x < src->width) {
                dst_data[new_y * src->width + new_x] =
                    src_data[i * src->width + j];
            }
        }
    }

    matrix_free(src);
    *src = dst;
}

static void matrix_upscale_u8(Matrix *src, u64 factor) {
    Matrix dst =
        matrix_new(src->width * factor, src->height * factor, U8_MATRIX, false);

    u8 *dst_data = (u8 *)dst.data;
    u8 *src_data = (u8 *)src->data;

    for (u64 i = 0; i < src->height; i++) {
        for (u64 j = 0; j < src->width; j++) {
            for (u64 k = 0; k < factor; k++) {
                for (u64 p = 0; p < factor; p++) {
                    dst_data[(factor * i + k) * dst.width + (factor * j + p)] =
                        src_data[i * src->width + j];
                }
            }
        }
    }

    matrix_free(src);
    *src = dst;
}

static void matrix_downscale_u8(Matrix *src, u64 factor) {
    Matrix dst =
        matrix_new(src->width / factor, src->height / factor, U8_MATRIX, false);

    u8 *dst_data = (u8 *)dst.data;
    u8 *src_data = (u8 *)src->data;

    for (u64 i = 0; i < dst.height; i++) {
        for (u64 j = 0; j < dst.width; j++) {
            u64 sum = 0;
            for (u64 k = 0; k < factor; k++) {
                for (u64 p = 0; p < factor; p++) {
                    sum += src_data[(factor * i + k) * src->width +
                                    (factor * j + p)];
                }
            }
            dst_data[i * dst.width + j] =
                (u8)roundf((f32)sum / (f32)(factor * factor));
        }
    }

    matrix_free(src);
    *src = dst;
}

static void matrix_upscale(Matrix *src, u64 factor) {
    if (src->matrix_type == U8_MATRIX) {
        matrix_upscale_u8(src, factor);
    }
}

static void matrix_u8_rotate2(Matrix *src, f32 degrees) {
    Matrix dst = matrix_new(src->width * 2, src->height * 2, U8_MATRIX, true);
    matrix_upscale_u8(src, 2);

    f32 cos_a = cosf(deg_to_rad(degrees));
    f32 sin_a = sinf(deg_to_rad(degrees));

    u8 *src_data = (u8 *)src->data;
    u8 *dst_data = (u8 *)dst.data;

    for (u64 i = 0; i < src->height; i++) {
        for (u64 j = 0; j < src->width; j++) {

            i64 old_x = (i64)src->width / 2 - (i64)j;
            i64 old_y = (i64)src->height / 2 - (i64)i;

            f32 new_x = (f32)old_x * cos_a - (f32)old_y * sin_a;
            f32 new_y = (f32)old_y * cos_a + (f32)old_x * sin_a;

            f32 new_j = (f32)src->width / 2.0f - new_x;
            f32 new_i = (f32)src->height / 2.0f - new_y;

            if ((u64)new_i < src->height && (u64)new_j < src->width) {
                dst_data[(u64)new_i * src->width + (u64)new_j] =
                    src_data[i * src->width + j];
            }
        }
    }
    matrix_free(src);
    matrix_downscale_u8(&dst, 2);
    *src = dst;
}

void matrix_rotate(Matrix *src, f32 degrees) {
    if (src->matrix_type == U8_MATRIX) {
        matrix_u8_rotate2(src, degrees);
    } else {
        matrix_u16_rotate(src, degrees);
    }
}
