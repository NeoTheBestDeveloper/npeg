#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "matrix.h"

static void _vertical_flip(u8_matrix *matrix) {
    for (size_t i = 0; i < matrix->height; i++) {
        for (size_t j = 0; j < matrix->width / 2; j++) {
            uint_fast8_t tmp = matrix->data[i][j];

            // just swap pixels from different edges of the matrix.
            matrix->data[i][j] = matrix->data[i][matrix->width - j - 1];
            matrix->data[i][matrix->width - j - 1] = tmp;
        }
    }
}

static void _horizontal_flip(u8_matrix *matrix) {
    for (size_t i = 0; i < matrix->height / 2; i++) {
        for (size_t j = 0; j < matrix->width; j++) {
            uint_fast8_t tmp = matrix->data[i][j];

            // just swap pixels from different edges of the matrix.
            matrix->data[i][j] = matrix->data[matrix->height - i - 1][j];
            matrix->data[matrix->height - i - 1][j] = tmp;
        }
    }
}

u8_matrix create_u8_matrix(size_t width, size_t height) {
    uint_fast8_t **data =
        (uint_fast8_t **)malloc(height * sizeof(uint_fast8_t *));
    for (size_t i = 0; i < height; i++)
        data[i] = (uint_fast8_t *)malloc(sizeof(uint_fast8_t) * width);
    u8_matrix matrix = {width, height, data};
    return matrix;
}

void free_u8_matrix(u8_matrix *matrix) {
    for (size_t i = 0; i < matrix->height; i++)
        free(matrix->data[i]);
    free(matrix->data);
}

void flip_u8_matrix(u8_matrix *matrix, enum directions direction) {
    switch (direction) {
    case VERTICAL:
        _vertical_flip(matrix);
        break;
    case HORIZONTAL:
        _horizontal_flip(matrix);
        break;
    }
}

void rotate_u8_matrix(u8_matrix *matrix, int degrees) {
    uint_fast8_t **old_data = matrix->data;
    int old_width = matrix->width;
    int old_height = matrix->height;

    // Swap width and height.
    if (degrees != 180) {
        matrix->width = old_height;
        matrix->height = old_width;
    }

    matrix->data =
        (uint_fast8_t **)malloc(sizeof(uint_fast8_t *) * matrix->height);
    for (int i = 0; i < matrix->height; i++) {
        matrix->data[i] =
            (uint_fast8_t *)malloc(sizeof(uint_fast8_t) * matrix->width);
        for (int j = 0; j < matrix->width; j++) {
            if (degrees == 90)
                matrix->data[i][j] = old_data[old_height - j - 1][i];
            else if (degrees == 270)
                matrix->data[i][j] = old_data[j][old_width - i - 1];
            else // 180
                matrix->data[i][j] =
                    old_data[old_height - i - 1][old_width - j - 1];
        }
    }

    // Free old memory.
    for (size_t i = 0; i < old_height; i++)
        free(old_data[i]);
    free(old_data);
}

void get_u8_area(u8_matrix *matrix, Vector *area, int center_x, int center_y,
                 int radius) {
    int area_top = 0;
    for (int i = center_y - radius; i < center_y + radius + 1; i++)
        for (int j = center_x - radius; j < center_x + radius + 1; j++)
            area->data[area_top++] = (double)matrix->data[i][j];
}
