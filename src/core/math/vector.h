#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>
#include <stdint.h>

typedef struct {
    size_t size;
    float *data;
} Vector;

float mult_vectors(Vector vector1, Vector vector2);

#endif // VECTOR_H
