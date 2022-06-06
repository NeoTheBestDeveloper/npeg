#ifndef MATRIX_H
#define MATRIX_H

#include <stddef.h>
#include <stdint.h>

#include "vector.h"

typedef struct {
    size_t width, height;
    uint_fast8_t **data;
} u8_matrix;

typedef struct {
    size_t width, height;
    uint_fast16_t **data;
} u16_matrix;

enum directions { VERTICAL, HORIZONTAL };

u8_matrix create_u8_matrix(size_t width, size_t height);
void rotate_u8_matrix(u8_matrix *matrix, int degrees);
void flip_u8_matrix(u8_matrix *matrix, enum directions direction);
void get_u8_area(u8_matrix *matrix, Vector *area, int center_x, int center_y,
                 int radius);
void free_u8_matrix(u8_matrix *matrix);

u16_matrix create_u16_matrix(size_t width, size_t height);
void rotate_u16_matrix(u16_matrix *matrix, int degrees);
void flip_u16_matrix(u16_matrix *matrix, enum directions direction);
void get_u16_area(u16_matrix *matrix, Vector *area, int center_x, int center_y,
                  int radius);
void free_u16_matrix(u16_matrix *matrix);

#endif // MATRIX_H
