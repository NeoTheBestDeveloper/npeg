#ifndef MATRIX_H
#define MATRIX_H

#include <stddef.h>
#include <stdint.h>

#include "vector.h"

typedef struct {
    size_t width, height;
    uint_fast16_t **data;
} Matrix;

enum directions { VERTICAL, HORIZONTAL };

Matrix create_matrix(size_t width, size_t height);
void free_matrix(Matrix *matrix);

void rotate_matrix(Matrix *matrix, int degrees);
void flip_matrix(Matrix *matrix, enum directions direction);
void get_area(Matrix *matrix, Vector *area, int center_x, int center_y,
              int radius);
void add_matrix(Matrix *dest, const Matrix *src);

#endif // MATRIX_H
