#ifndef H_INTERPOLATION
#define H_INTERPOLATION

#include "../math/matrix.h"

typedef enum {
    INTER_NONE = 0,
    INTER_LINIAR = 1,
    INTER_AVG = 2,
    // INTER_LANCZOS = 2,
    // INTER_CUBIC = 3,
} Interpolation;

void matrix_bilinear_inter(Matrix *src);
void matrix_avg_inter(Matrix *src);

// FIXME: void matrix_lanczos_inter(Matrix *src);
// TODO: void matrix_bicubic_inter(Matrix *src);

#endif // !H_INTERPOLATION
