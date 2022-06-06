#include "vector.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

double mult_vectors(Vector *vec1, Vector *vec2) {
    double result = 0;
    for (size_t i = 0; i < vec1->size; i++)
        result += vec1->data[i] * vec2->data[i];
    return result;
}

Vector create_vector(size_t size) {
    double *data = (double *)malloc(sizeof(double) * size);
    Vector vec = {size, data};
    return vec;
}

double sum_vector(Vector *vec) {
    double result = 0;
    for (size_t i = 0; i < vec->size; i++)
        result += vec->data[i];
    return result;
}

int _comp(const void *elem1, const void *elem2) {
    int f = *((double *)elem1);
    int s = *((double *)elem2);
    if (f > s)
        return 1;
    if (f < s)
        return -1;
    return 0;
}

void sort_vector(Vector *vec) {
    qsort(vec->data, vec->size, sizeof(double), _comp);
}

void free_vector(Vector *vec) { free(vec->data); }
