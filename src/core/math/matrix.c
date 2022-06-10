#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "matrix.h"

static void _vertical_flip(Matrix *matrix) {
    for (size_t i = 0; i < matrix->height; i++) {
        for (size_t j = 0; j < matrix->width / 2; j++) {
            int32_t tmp = matrix->data[i][j];

            // just swap pixels from different edges of the matrix.
            matrix->data[i][j] = matrix->data[i][matrix->width - j - 1];
            matrix->data[i][matrix->width - j - 1] = tmp;
        }
    }
}

static void _horizontal_flip(Matrix *matrix) {
    for (size_t i = 0; i < matrix->height / 2; i++) {
        for (size_t j = 0; j < matrix->width; j++) {
            int32_t tmp = matrix->data[i][j];

            // just swap pixels from different edges of the matrix.
            matrix->data[i][j] = matrix->data[matrix->height - i - 1][j];
            matrix->data[matrix->height - i - 1][j] = tmp;
        }
    }
}

Matrix create_matrix(size_t width, size_t height) {
    int32_t **data = (int32_t **)malloc(height * sizeof(int32_t *));
    for (size_t i = 0; i < height; i++)
        data[i] = (int32_t *)malloc(sizeof(int32_t) * width);
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
    int new_width = (degrees == 180) ? matrix->width : matrix->height;
    int new_height = (degrees == 180) ? matrix->height : matrix->width;

    int32_t **new_data = (int32_t **)malloc(sizeof(int32_t *) * new_height);
    for (int i = 0; i < new_height; i++) {
        new_data[i] = (int32_t *)malloc(sizeof(int32_t) * new_width);
        for (int j = 0; j < new_width; j++) {
            if (degrees == 90) {
                new_data[i][j] = matrix->data[matrix->height - j - 1][i];
            } else if (degrees == 270)
                new_data[i][j] = matrix->data[j][matrix->width - i - 1];
            else // 180
                new_data[i][j] =
                    matrix->data[matrix->height - i - 1][matrix->width - j - 1];
        }
    }

    // Free old data.
    for (int i = 0; i < matrix->height; i++)
        free(matrix->data[i]);
    free(matrix->data);

    matrix->data = new_data;
    matrix->height = new_height;
    matrix->width = new_width;
}

void get_area(Matrix *matrix, Vector *area, int center_x, int center_y,
              int radius) {
    int area_top = 0;
    for (size_t i = center_y - radius; i < center_y + radius + 1; i++)
        for (size_t j = center_x - radius; j < center_x + radius + 1; j++)
            area->data[area_top++] = matrix->data[i][j];
}

void add_matrix(Matrix *dest, const Matrix *src) {
    for (size_t i = 0; i < dest->height; i++)
        for (size_t j = 0; j < dest->width; j++)
            dest->data[i][j] += src->data[i][j];
}
