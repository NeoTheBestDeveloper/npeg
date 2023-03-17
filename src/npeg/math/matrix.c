#include <malloc.h>

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
