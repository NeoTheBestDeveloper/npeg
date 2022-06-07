#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "filter.h"

#include "../math/matrix.h"
#include "../math/vector.h"

static void _gen_gaussian_filter(Vector *filter, int filter_size, double dev) {
    int filter_top = 0;
    int filter_padding = (filter_size - 1) / 2;

    for (int i = -filter_padding; i < filter_size - filter_padding; i++)
        for (int j = -filter_padding; j < filter_size - filter_padding; j++)
            filter->data[filter_top++] =
                (1.0 / (2.0 * M_PI * pow(dev, 2))) *
                exp(-(pow(j, 2.0) + pow(i, 2.0)) / (2 * pow(dev, 2)));
}

static void _gen_unsharp_filter(Vector *filter, int filter_size, double dev) {
    int filter_top = 0;
    int filter_padding = (filter_size - 1) / 2;

    for (int i = -filter_padding; i < filter_size - filter_padding; i++)
        for (int j = -filter_padding; j < filter_size - filter_padding; j++)
            filter->data[filter_top++] =
                (-1.0 / (M_PI * pow(dev, 4))) *
                (1.0 - ((pow(i, 2.0) + pow(j, 2.0)) / (2.0 * pow(dev, 2.0)))) *
                exp(-(pow(j, 2.0) + pow(i, 2.0)) / (2.0 * pow(dev, 2.0)));
}

static void _filter_image(Matrix *matrix, int filter_size,
                          void gen_filter(Vector *filter_, int filter_size,
                                          double dev)) {
    int filter_padding = (filter_size - 1) / 2;
    int area_size = filter_size * filter_size;

    Vector area = create_vector(area_size);

    Vector filter = create_vector(area_size);
    gen_filter(&filter, filter_size, 1.0);

    uint_fast16_t **new_data =
        (uint_fast16_t **)malloc(matrix->height * sizeof(uint_fast16_t *));
    for (size_t i = 0; i < matrix->height; i++)
        new_data[i] =
            (uint_fast16_t *)malloc(sizeof(uint_fast16_t) * matrix->width);

    for (int i = filter_padding; i < matrix->height - filter_padding; i++) {
        for (int j = filter_padding; j < matrix->width - filter_padding; j++) {
            get_area(matrix, &area, j, i, filter_padding);
            new_data[i][j] = round(mult_vectors(&filter, &area));
        }
    }

    free_vector(&area);
    free_vector(&filter);

    uint_fast16_t **old_data = matrix->data;
    if (gen_filter == _gen_unsharp_filter) {
        for (size_t i = 0; i < matrix->height; i++)
            for (size_t j = 0; j < matrix->width; j++)
                matrix->data[i][j] -= new_data[i][j];
        // Free old data.
        for (size_t i = 0; i < matrix->height; i++)
            free(new_data[i]);
        free(new_data);
    } else {
        matrix->data = new_data;
        // Free old data.
        for (size_t i = 0; i < matrix->height; i++)
            free(old_data[i]);
        free(old_data);
    }
}

static void _box_filter(Matrix *matrix, int filter_size) {
    int filter_padding = (filter_size - 1) / 2;
    double area_size = filter_size * filter_size;
    Vector area = create_vector(area_size);

    uint_fast16_t **new_data =
        (uint_fast16_t **)malloc(matrix->height * sizeof(uint_fast16_t *));
    for (size_t i = 0; i < matrix->height; i++)
        new_data[i] =
            (uint_fast16_t *)malloc(sizeof(uint_fast16_t) * matrix->width);

    for (int i = filter_padding; i < matrix->height - filter_padding; i++) {
        for (int j = filter_padding; j < matrix->width - filter_padding; j++) {
            get_area(matrix, &area, j, i, filter_padding);
            new_data[i][j] = round(sum_vector(&area) / area_size);
        }
    }

    free_vector(&area);

    // Free old data.
    for (size_t i = 0; i < matrix->height; i++)
        free(matrix->data[i]);
    free(matrix->data);

    matrix->data = new_data;
}

static void _med_filter(Matrix *matrix, int filter_size) {
    int filter_padding = (filter_size - 1) / 2;
    int area_size = filter_size * filter_size;
    Vector area = create_vector(area_size);

    uint_fast16_t **new_data =
        (uint_fast16_t **)malloc(matrix->height * sizeof(uint_fast16_t *));
    for (size_t i = 0; i < matrix->height; i++)
        new_data[i] =
            (uint_fast16_t *)malloc(sizeof(uint_fast16_t) * matrix->width);

    for (int i = filter_padding; i < matrix->height - filter_padding; i++)
        for (int j = filter_padding; j < matrix->width - filter_padding; j++) {
            get_area(matrix, &area, j, i, filter_padding);
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

void filter_image(Matrix *matrix, int filter_size, enum filters filter) {
    switch (filter) {
    case BOX_FILTER:
        _box_filter(matrix, filter_size);
        break;
    case MED_FILTER:
        _med_filter(matrix, filter_size);
        break;
    case GAUSSIAN_FILTER:
        _filter_image(matrix, filter_size, _gen_gaussian_filter);
        break;
    case UNSHARP_FILTER:
        _filter_image(matrix, filter_size, _gen_unsharp_filter);
        break;
    }
}
