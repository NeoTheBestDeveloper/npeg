#include "vector.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

float mult_vectors(Vector *vec1, Vector *vec2) {
    float result = 0;
    for (size_t i = 0; i < vec1->size; i++)
        result += vec1->data[i] * vec2->data[i];
    return result;
}

Vector create_vector(size_t size) {
    float *data = (float *)malloc(sizeof(float) * size);
    Vector vec = {size, data};
    return vec;
}

float sum_vector(Vector *vec) {
    float result = 0;
    // printf("sum vec: %f", result);
    for (size_t i = 0; i < vec->size; i++) {
        result += vec->data[i];
        // printf("sum vec: %f\n", result);
        // printf("sum vec element: %f\n", vec->data[i]);
    }
    return result;
}

int _comp(const void *elem1, const void *elem2) {
    int f = *((float *)elem1);
    int s = *((float *)elem2);
    if (f > s)
        return 1;
    if (f < s)
        return -1;
    return 0;
}

void sort_vector(Vector *vec) {
    qsort(vec->data, vec->size, sizeof(float), _comp);
}

void free_vector(Vector *vec) { free(vec->data); }
