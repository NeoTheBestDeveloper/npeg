#include <immintrin.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "../math/matrix.h"
#include "../math/trig.h"
#include "algorithms.h"
#include "benchmark.h"
#include "types.h"

#define SCALE_FACTOR (2)

static void matrix_u8_rotate(const Matrix *src, Matrix *dst, f32 sin_a,
                             f32 cos_a) {
  u8 *src_data = (u8 *)src->data;
  u8 *dst_data = (u8 *)dst->data;

  f32 height_half = (f32)(src->height >> 1);
  f32 width_half = (f32)(src->width >> 1);
  i64 src_height = src->height;
  i64 src_width = src->width;

  for (i64 i = src_height - 1; i != 0; --i) {
    f32 old_y = height_half - (f32)i;
    i64 raws_offset = i * src_width;

    for (i64 j = src_width - 1; j != 0; --j) {
      f32 old_x = width_half - (f32)j;

      f32 new_x = old_x * cos_a - old_y * sin_a;
      f32 new_y = old_y * cos_a + old_x * sin_a;

      i64 new_j = (i64)(width_half - new_x);
      i64 new_i = (i64)(height_half - new_y);

      if (new_i < src_height && new_j <= src_width && new_j >= 0 && new_i > 0) {
        dst_data[new_i * src_width + new_j] = src_data[raws_offset + j];
      }
    }
  }
}

static void matrix_u8_rotate_avx(const Matrix *src, Matrix *dst, f32 sin_a,
                                 f32 cos_a) {
  u8 *src_data = (u8 *)src->data;
  u8 *dst_data = (u8 *)dst->data;

  f32 height_half = (f32)src->height / 2.f;
  f32 width_half = (f32)src->width / 2.f;

  i64 src_height = src->height;
  i64 src_width = src->width;

  __m256 _old_x_offsets =
      _mm256_setr_ps(0.f, 1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f);

  __m256 _cos_a = _mm256_set1_ps(cos_a);
  __m256 _sin_a = _mm256_set1_ps(sin_a);

  __m256 _height_half = _mm256_set1_ps(height_half);
  __m256 _width_half = _mm256_set1_ps(width_half);

  for (i64 i = src_height - 1; i != 0; --i) {
    i64 rows_offset = i * src_width;
    __m256 _old_y = _mm256_sub_ps(_height_half, _mm256_set1_ps((f32)i));

    for (i64 j = src_width; j >= 0; j -= 8) {
      __m256 _old_x = _mm256_set1_ps((f32)j);
      _old_x = _mm256_add_ps(_old_x, _old_x_offsets);
      _old_x = _mm256_sub_ps(_width_half, _old_x); // _old_x

      __m256 _new_x = _mm256_mul_ps(_old_y, _sin_a);
      _new_x = _mm256_fmsub_ps(_cos_a, _old_x, _new_x);

      __m256 _new_y = _mm256_mul_ps(_old_x, _sin_a);
      _new_y = _mm256_fmadd_ps(_old_y, _cos_a, _new_y);

      _new_x = _mm256_sub_ps(_width_half, _new_x);
      _new_y = _mm256_sub_ps(_height_half, _new_y);

      f32 *new_js = (f32 *)&_new_x;
      f32 *new_is = (f32 *)&_new_y;

      for (i32 k = 0; k < 8; ++k) {
        i64 new_i = (i64)new_is[k];
        i64 new_j = (i64)new_js[k];

        if (new_i < src_height && new_j < src_width && new_j >= 0 &&
            new_i >= 0) {
          dst_data[new_i * src_width + new_j] = src_data[rows_offset + j + k];
        }
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

      if ((i64)new_i < src->height && (i64)new_j < src->width && new_i > 0 &&
          new_j > 0) {
        dst_data[(i64)new_i * src->width + (i64)new_j] =
            src_data[i * src->width + j];
      }
    }
  }
}

static void matrix_rotation_post_process(Matrix *src,
                                         PostProcess post_process) {
  switch (post_process) {
  case UP_DOWN_SCALE:
    benchmark(matrix downscale) { matrix_downscale(src, SCALE_FACTOR); }
    break;
  case BILINEAR_INTER:
    benchmark(Bilinear interpolation) { matrix_bilinear_inter(src); }
    break;
  case LANCZOS_INTER:
    benchmark(Lanczos kernel interpolation) { matrix_lanczos_inter(src); }
    break;
  case AVG_INTER:
    benchmark(Avg interpolation) { matrix_avg_inter(src); }
    break;
  case NONE_PROCESS:
    return;
  }
}

void matrix_rotate(Matrix *src, f32 degrees, PostProcess post_process) {
  Matrix dst;
  if (post_process == UP_DOWN_SCALE) {
    dst = matrix_new(src->width * SCALE_FACTOR, src->height * SCALE_FACTOR,
                     src->matrix_type, true);
    benchmark(matrix upscale) { matrix_upscale(src, SCALE_FACTOR); }
  } else {
    dst = matrix_new(src->width, src->height, src->matrix_type, true);
  }

  f32 cos_a = cosf(deg_to_rad(degrees));
  f32 sin_a = sinf(deg_to_rad(degrees));

  benchmark(matrix avx rotation) {
    if (src->matrix_type == U8_MATRIX) {
      matrix_u8_rotate_avx(src, &dst, sin_a, cos_a);
    } else {
      matrix_u16_rotate(src, &dst, sin_a, cos_a);
    }
  }

  // benchmark(matrix rotation) {
  //     if (src->matrix_type == U8_MATRIX) {
  //         matrix_u8_rotate(src, &dst, sin_a, cos_a);
  //     } else {
  //         matrix_u16_rotate(src, &dst, sin_a, cos_a);
  //     }
  // }

  matrix_free(src);
  matrix_rotation_post_process(&dst, post_process);

  *src = dst;
}
