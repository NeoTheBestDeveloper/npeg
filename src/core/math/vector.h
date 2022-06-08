#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>
#include <stdint.h>

typedef struct {
    size_t size;
    float *data;
} Vector;

Vector *create_vector(size_t size);
void free_vector(Vector *vec);

float mult_vectors(Vector *vec1, Vector *vec2);
float sum_vector(Vector *vec);
void sort_vector(Vector *vec);

#endif // VECTOR_H
