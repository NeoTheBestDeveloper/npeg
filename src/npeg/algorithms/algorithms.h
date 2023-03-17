#ifndef H_ALGORITHMS
#define H_ALGORITHMS

#include "../math/matrix.h"
#include "post_process.h"
#include "types.h"

void matrix_bilinear_inter(Matrix *src);
void matrix_lanczos_inter(Matrix *src);
void matrix_avg_inter(Matrix *src);
void matrix_rotate(Matrix *src, f32 degrees, PostProcess post_process);

void matrix_downscale(Matrix *src, i64 factor);
void matrix_upscale(Matrix *src, i64 factor);

#endif // !H_ALGORITHMS
