#ifndef H_U8_MATRIX
#define H_U8_MATRIX

#include <types.h>

typedef struct {
    u64 height, width;
    u8 *data;
} matrix_u8;

matrix_u8 matrix_u8_new(u64 width, u64 height);
void matrix_u8_free(matrix_u8 *matrix);

void matrix_u8_rotate(matrix_u8 *matrix, f32 degrees);

#endif // !H_U8_MATRIX
