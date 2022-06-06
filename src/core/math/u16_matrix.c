#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "matrix.h"

static void _vertical_flip(u16_matrix *matrix) {
    for (size_t i = 0; i < matrix->height; i++) {
        for (size_t j = 0; j < matrix->width / 2; j++) {
            uint_fast16_t tmp = matrix->data[i][j];

            // just swap pixels from different edges of the matrix.
            matrix->data[i][j] = matrix->data[i][matrix->width - j - 1];
            matrix->data[i][matrix->width - j - 1] = tmp;
        }
    }
}

static void _horizontal_flip(u16_matrix *matrix) {
    for (size_t i = 0; i < matrix->height / 2; i++) {
        for (size_t j = 0; j < matrix->width; j++) {
            uint_fast16_t tmp = matrix->data[i][j];

            // just swap pixels from different edges of the matrix.
            matrix->data[i][j] = matrix->data[matrix->height - i - 1][j];
            matrix->data[matrix->height - i - 1][j] = tmp;
        }
    }
}

u16_matrix create_u16_matrix(size_t width, size_t height) {
    uint_fast16_t **data =
        (uint_fast16_t **)malloc(height * sizeof(uint_fast16_t *));
    for (size_t i = 0; i < height; i++)
        data[i] = (uint_fast16_t *)malloc(sizeof(uint_fast16_t) * width);
    u16_matrix matrix = {width, height, data};
    return matrix;
}

void free_u16_matrix(u16_matrix *matrix) {
    for (size_t i = 0; i < matrix->height; i++)
        free(matrix->data[i]);
    free(matrix->data);
}

void flip_u16_matrix(u16_matrix *matrix, enum directions direction) {
    switch (direction) {
    case VERTICAL:
        _vertical_flip(matrix);
    case HORIZONTAL:
        _horizontal_flip(matrix);
    }
}

void rotate_u16_matrix(u16_matrix *matrix, int degrees) {
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
    free_u16_matrix(matrix);

    // Swap width and height.
    if (degrees != 180) {
        matrix->width = old_height;
        matrix->height = old_width;
    }

    matrix->data = new_data;
}
