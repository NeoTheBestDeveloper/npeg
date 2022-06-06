#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "filter.h"

#include "../math/matrix.h"
#include "../math/vector.h"

// static void _gen_gaussian_filter(double *filter, int filter_size, double dev)
// {
//     int filter_top = 0;
//     int filter_padding = (filter_size - 1) / 2;
//     for (int i = -filter_padding; i < filter_size - filter_padding; i++)
//         for (int j = -filter_padding; j < filter_size - filter_padding; j++)
//             filter[filter_top++] =
//                 (1.0 / (sqrt(2.0 * M_PI) * dev)) *
//                 exp(-(pow(i, 2) + pow(j, 2)) / (2.0 * pow(dev, 2)));
// }

// static void _gaussian_filter(u8_matrix *matrix, int filter_size) {
//     int filter_padding = (filter_size - 1) / 2;
//     int area_size = filter_size * filter_size;
//     Vector area = create_vector(area_size);
//
//     double *gaussian_filter = (double *)malloc(filter_size * filter_size);
//     _gen_gaussian_filter(gaussian_filter, filter_size, 1.0);
//
//     for (int i = filter_padding; i < matrix->height - filter_padding; i++) {
//         for (int j = filter_padding; j < matrix->width - filter_padding; j++)
//         {
//             get_u8_area(matrix, &area, j, i, filter_padding);
//             double result = mult_vectors(gaussian_filter, area);
//             matrix->data[i][j] = (result > 0.5) ? '1' : '0';
//         }
//     }
//
//     free_vector(&area);
//     free(gaussian_filter);
// }

// static void _unsharp_filter(u8_matrix *matrix, int filter_size) {}

static void _box_filter(u8_matrix *matrix, int filter_size) {
    int filter_padding = (filter_size - 1) / 2;
    float area_size = filter_size * filter_size;
    Vector area = create_vector(area_size);

    uint_fast8_t **new_data =
        (uint_fast8_t **)malloc(matrix->height * sizeof(uint_fast8_t *));
    for (size_t i = 0; i < matrix->height; i++)
        new_data[i] =
            (uint_fast8_t *)malloc(sizeof(uint_fast8_t) * matrix->width);

    for (int i = filter_padding; i < matrix->height - filter_padding; i++) {
        for (int j = filter_padding; j < matrix->width - filter_padding; j++) {
            get_u8_area(matrix, &area, j, i, filter_padding);
            new_data[i][j] = roundf(sum_vector(&area) / area_size);
        }
    }

    free_vector(&area);

    // Free old data.
    for (size_t i = 0; i < matrix->height; i++)
        free(matrix->data[i]);
    free(matrix->data);

    matrix->data = new_data;
}

static void _med_filter(u8_matrix *matrix, int filter_size) {
    int filter_padding = (filter_size - 1) / 2;
    int area_size = filter_size * filter_size;
    Vector area = create_vector(area_size);

    uint_fast8_t **new_data =
        (uint_fast8_t **)malloc(matrix->height * sizeof(uint_fast8_t *));
    for (size_t i = 0; i < matrix->height; i++)
        new_data[i] =
            (uint_fast8_t *)malloc(sizeof(uint_fast8_t) * matrix->width);

    for (int i = filter_padding; i < matrix->height - filter_padding; i++)
        for (int j = filter_padding; j < matrix->width - filter_padding; j++) {
            get_u8_area(matrix, &area, j, i, filter_padding);
            sort_vector(&area);
            new_data[i][j] = area.data[area.size / 2 + 1];
        }

    free_vector(&area);

    // Free old data.
    for (size_t i = 0; i < matrix->height; i++)
        free(matrix->data[i]);
    free(matrix->data);

    matrix->data = new_data;
}

void filter_image(u8_matrix *matrix, int filter_size, enum filters filter) {
    switch (filter) {
    case BOX_FILTER:
        _box_filter(matrix, filter_size);
        break;
    case MED_FILTER:
        _med_filter(matrix, filter_size);
        break;
        // case GAUSSIAN_FILTER:
        //     _gaussian_filter(image, filter_size);
        //     break;
        // case UNSHARP_FILTER:
        //     _unsharp_filter(image, filter_size);
        break;
    }
}
