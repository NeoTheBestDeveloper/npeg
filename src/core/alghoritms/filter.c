#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "filter.h"

#include "../math/matrix.h"
#include "../math/vector.h"

static Vector *_gen_gaussian_kernel(int kernel_size, double sigma) {
    Vector *kernel = create_vector(kernel_size * kernel_size);
    int kernel_top = 0;
    int kernel_padding = (kernel_size - 1) / 2;

    for (int i = -kernel_padding; i < kernel_size - kernel_padding; i++)
        for (int j = -kernel_padding; j < kernel_size - kernel_padding; j++)
            kernel->data[kernel_top++] =
                (1.0 / (2.0 * M_PI * pow(sigma, 2))) *
                exp(-(pow(j, 2.0) + pow(i, 2.0)) / (2 * pow(sigma, 2)));
    return kernel;
}

static Vector *_gen_unsharp_kernel(int kernel_size, double sigma) {
    Vector *kernel = create_vector(kernel_size * kernel_size);
    int kernel_top = 0;
    int kernel_padding = (kernel_size - 1) / 2;

    for (int i = -kernel_padding; i < kernel_size - kernel_padding; i++)
        for (int j = -kernel_padding; j < kernel_size - kernel_padding; j++)
            kernel->data[kernel_top++] =
                (-1.0 / (M_PI * pow(sigma, 4))) *
                (1.0 -
                 ((pow(i, 2.0) + pow(j, 2.0)) / (2.0 * pow(sigma, 2.0)))) *
                exp(-(pow(j, 2.0) + pow(i, 2.0)) / (2.0 * pow(sigma, 2.0)));
    return kernel;
}

static int32_t _process_pixel(int32_t pixel, enum filters filter,
                              int max_color_value, Vector *kernel,
                              Vector *area) {
    int32_t result;
    if (filter == BOX_FILTER)
        result = round(sum_vector(area) / (double)area->size);
    else if (filter == MED_FILTER) {
        sort_vector(area);
        result = area->data[area->size / 2 + 1];
    } else if (filter == GAUSSIAN_FILTER)
        result = round(mult_vectors(kernel, area));
    else if (filter == UNSHARP_FILTER)
        result = round((double)pixel - mult_vectors(kernel, area));

    // if (result < 0)
    //     return 0;
    // else if (result > max_color_value)
    //     return max_color_value;
    // else
    printf("%d", result);
    return result;
}

static Vector *_gen_kernel(enum filters filter, int kernel_size, double sigma) {
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

void filter_image(Matrix *matrix, int max_color_value, int filter_size,
                  enum filters filter, double sigma) {
    int filter_padding = (filter_size - 1) / 2;
    double area_size = filter_size * filter_size;

    Vector *area = create_vector(area_size);
    Vector *kernel = _gen_kernel(filter, area_size, sigma);

    int32_t **new_data = (int32_t **)malloc(matrix->height * sizeof(int32_t *));
    for (size_t i = 0; i < matrix->height; i++)
        new_data[i] = (int32_t *)malloc(sizeof(int32_t) * matrix->width);

    for (int i = filter_padding; i < matrix->height - filter_padding; i++) {
        for (int j = filter_padding; j < matrix->width - filter_padding; j++) {
            get_area(matrix, area, j, i, filter_padding);
            if (filter == MED_FILTER) {
                sort_vector(area);
                new_data[i][j] = (int32_t)area->data[area->size / 2 + 1];
            }
            if (filter == BOX_FILTER) {

                new_data[i][j] = round(sum_vector(area) / (double)area->size);
            }
            if (filter == GAUSSIAN_FILTER) {

                new_data[i][j] = round(mult_vectors(kernel, area));
            }
            if (filter == UNSHARP_FILTER) {

                new_data[i][j] =
                    round((double)new_data[i][j] - mult_vectors(kernel, area));
            }
            printf("%d ", new_data[i][j]);
        }
    }

    free_vector(area);
    if (kernel != NULL)
        free_vector(kernel);

    // Free old data.
    for (size_t i = 0; i < matrix->height; i++)
        free(matrix->data[i]);
    free(matrix->data);

    matrix->data = new_data;
}
