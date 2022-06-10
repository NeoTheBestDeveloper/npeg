#ifndef MATRIX_H
#define MATRIX_H

#include <stdint.h>

#include "vector.h"

enum directions { VERTICAL, HORIZONTAL };

typedef struct {
    size_t width, height;
    int32_t **data;
} Matrix;

Matrix create_matrix(size_t width, size_t height);
void free_matrix(Matrix *matrix);

void rotate_matrix(Matrix *matrix, int degrees);
void flip_matrix(Matrix *matrix, enum directions direction);
void get_area(Matrix *matrix, Vector *area, int center_x, int center_y,
              int radius);
void add_matrix(Matrix *dest, const Matrix *src);

#endif // MATRIX_H
