#ifndef H_U8_MATRIX
#define H_U8_MATRIX

#include <stdbool.h>
#include <types.h>

typedef enum {
    U8_MATRIX,
    U16_MATRIX,
} MatrixType;

// Matrix work with unsigned integer types as u8 and u16.
typedef struct {
    u64 height, width;
    void *data;
    MatrixType matrix_type;
} Matrix;

Matrix matrix_new(u64 width, u64 height, MatrixType matrix_type,
                  bool fill_zeroes);
void matrix_free(Matrix *matrix);

void matrix_rotate(Matrix *matrix, f32 degrees);

#endif // !H_U8_MATRIX
