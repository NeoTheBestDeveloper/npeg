#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "matrix.h"

static void _vertical_flip(Matrix *matrix) {
    for (size_t i = 0; i < matrix->height; i++) {
        for (size_t j = 0; j < matrix->width / 2; j++) {
            uint_fast16_t tmp = matrix->data[i][j];

            // just swap pixels from different edges of the matrix.
            matrix->data[i][j] = matrix->data[i][matrix->width - j - 1];
            matrix->data[i][matrix->width - j - 1] = tmp;
        }
    }
}

static void _horizontal_flip(Matrix *matrix) {
    for (size_t i = 0; i < matrix->height / 2; i++) {
        for (size_t j = 0; j < matrix->width; j++) {
            uint_fast16_t tmp = matrix->data[i][j];

            // just swap pixels from different edges of the matrix.
            matrix->data[i][j] = matrix->data[matrix->height - i - 1][j];
            matrix->data[matrix->height - i - 1][j] = tmp;
        }
    }
}

Matrix create_matrix(size_t width, size_t height) {
    uint_fast16_t **data =
        (uint_fast16_t **)malloc(height * sizeof(uint_fast16_t *));
    for (size_t i = 0; i < height; i++)
        data[i] = (uint_fast16_t *)malloc(sizeof(uint_fast16_t) * width);
    Matrix matrix = {width, height, data};
    return matrix;
}

void free_matrix(Matrix *matrix) {
    for (size_t i = 0; i < matrix->height; i++)
        free(matrix->data[i]);
    free(matrix->data);
}

void flip_matrix(Matrix *matrix, enum directions direction) {
    switch (direction) {
    case VERTICAL:
        _vertical_flip(matrix);
    case HORIZONTAL:
        _horizontal_flip(matrix);
    }
}

void rotate_matrix(Matrix *matrix, int degrees) {
    int old_width = matrix->width;
    int old_height = matrix->height;

    uint_fast16_t **new_data =
        (uint_fast16_t **)malloc(sizeof(uint_fast16_t *) * matrix->height);
    for (int i = 0; i < matrix->height; i++) {
        new_data[i] =
            (uint_fast16_t *)malloc(sizeof(uint_fast16_t) * matrix->width);
        for (int j = 0; j < matrix->width; j++) {
            if (degrees == 90)
                new_data[i][j] = matrix->data[old_height - j - 1][i];
            else if (degrees == 270)
                new_data[i][j] = matrix->data[j][old_width - i - 1];
            else // 180
                new_data[i][j] =
                    matrix->data[old_height - i - 1][old_width - j - 1];
        }
    }

    // Free old memory.
    free_matrix(matrix);

    // Swap width and height.
    if (degrees != 180) {
        matrix->width = old_height;
        matrix->height = old_width;
    }

    matrix->data = new_data;
}

void get_area(Matrix *matrix, Vector *area, int center_x, int center_y,
              int radius) {
    int area_top = 0;
    for (int i = center_y - radius; i < center_y + radius + 1; i++)
        for (int j = center_x - radius; j < center_x + radius + 1; j++)
            area->data[area_top++] = (double)matrix->data[i][j];
}

void add_matrix(Matrix *dest, const Matrix *src) {
    for (size_t i = 0; i < dest->height; i++)
        for (size_t j = 0; j < dest->width; j++)
            dest->data[i][j] += src->data[i][j];
}
