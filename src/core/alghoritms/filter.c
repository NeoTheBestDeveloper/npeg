#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "filter.h"

#include "../math/matrix.h"
#include "../math/vector.h"

static Vector *_gen_gaussian_kernel(int kernel_size, float sigma) {
    Vector *kernel = create_vector(kernel_size * kernel_size);
    int kernel_top = 0;
    int kernel_padding = (kernel_size - 1) / 2;

    for (int i = -kernel_padding; i < kernel_size - kernel_padding; i++)
        for (int j = -kernel_padding; j < kernel_size - kernel_padding; j++)
            kernel->data[kernel_top++] =
                (1.0 / (2.0 * M_PI * powf(sigma, 2))) *
                exp(-(powf(j, 2.0) + powf(i, 2.0)) / (2 * powf(sigma, 2)));
    return kernel;
}

static Vector *_gen_unsharp_kernel(int kernel_size, float sigma) {
    Vector *kernel = create_vector(kernel_size * kernel_size);
    int kernel_top = 0;
    int kernel_padding = (kernel_size - 1) / 2;

    for (int i = -kernel_padding; i < kernel_size - kernel_padding; i++)
        for (int j = -kernel_padding; j < kernel_size - kernel_padding; j++)
            kernel->data[kernel_top++] =
                (-1.0 / (M_PI * powf(sigma, 4))) *
                (1.0 -
                 ((powf(i, 2.0) + powf(j, 2.0)) / (2.0 * powf(sigma, 2.0)))) *
                exp(-(powf(j, 2.0) + powf(i, 2.0)) / (2.0 * powf(sigma, 2.0)));
    return kernel;
}

static int _process_pixel(uint_fast16_t pixel, enum filters filter,
                          int max_color_value, Vector *kernel, Vector *area) {
    int result;
    switch (filter) {
    case BOX_FILTER:
        result = roundf(sum_vector(area) / (float)area->size);
        break;
    case MED_FILTER:
        sort_vector(area);
        result = area->data[area->size / 2 + 1];
        break;
    case GAUSSIAN_FILTER:
        result = roundf(mult_vectors(kernel, area));
        break;
    case UNSHARP_FILTER:
        result = roundf((float)pixel - mult_vectors(kernel, area));
        break;
    }

    if (result < 0)
        return 0;
    else if (result > max_color_value)
        return max_color_value;
    else
        return result;
}

static Vector *_gen_kernel(enum filters filter, int kernel_size, float sigma) {
    switch (filter) {
    case BOX_FILTER:
        return NULL;
    case MED_FILTER:
        return NULL;
    case GAUSSIAN_FILTER:
        return _gen_gaussian_kernel(kernel_size, sigma);
    case UNSHARP_FILTER:
        return _gen_unsharp_kernel(kernel_size, sigma);
    }
};

void filter_normal_image(u8_matrix *matrix, int max_color_value,
                         int filter_size, enum filters filter, float sigma) {
    int filter_padding = (filter_size - 1) / 2;
    float area_size = filter_size * filter_size;

    Vector *area = create_vector(area_size);
    Vector *kernel = _gen_kernel(filter, area_size, sigma);

    uint_fast8_t **new_data =
        (uint_fast8_t **)malloc(matrix->height * sizeof(uint_fast8_t *));
    for (size_t i = 0; i < matrix->height; i++)
        new_data[i] =
            (uint_fast8_t *)malloc(sizeof(uint_fast8_t) * matrix->width);

    for (int i = filter_padding; i < matrix->height - filter_padding; i++) {
        for (int j = filter_padding; j < matrix->width - filter_padding; j++) {
            get_u8_area(matrix, area, j, i, filter_padding);
            new_data[i][j] = _process_pixel(new_data[i][j], filter,
                                            max_color_value, kernel, area);
        }
    }

    free_vector(area);
    free_vector(kernel);

    // Free old data.
    for (size_t i = 0; i < matrix->height; i++)
        free(matrix->data[i]);
    free(matrix->data);

    matrix->data = new_data;
}

void filter_extended_image(u16_matrix *matrix, int max_color_value,
                           int filter_size, enum filters filter, float sigma) {
    int filter_padding = (filter_size - 1) / 2;
    float area_size = filter_size * filter_size;

    Vector *area = create_vector(area_size);
    Vector *kernel = _gen_kernel(filter, area_size, sigma);

    uint_fast16_t **new_data =
        (uint_fast16_t **)malloc(matrix->height * sizeof(uint_fast16_t *));
    for (size_t i = 0; i < matrix->height; i++)
        new_data[i] =
            (uint_fast16_t *)malloc(sizeof(uint_fast16_t) * matrix->width);

    for (int i = filter_padding; i < matrix->height - filter_padding; i++) {
        for (int j = filter_padding; j < matrix->width - filter_padding; j++) {
            get_u16_area(matrix, area, j, i, filter_padding);
            new_data[i][j] = _process_pixel(new_data[i][j], filter,
                                            max_color_value, kernel, area);
        }
    }

    free_vector(area);
    free_vector(kernel);

    // Free old data.
    for (size_t i = 0; i < matrix->height; i++)
        free(matrix->data[i]);
    free(matrix->data);

    matrix->data = new_data;
}
