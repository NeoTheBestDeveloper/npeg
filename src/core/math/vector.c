#include "vector.h"

float mult_vectors(Vector vec1, Vector vec2) {
    float result = 0;
    for (size_t i = 0; i < vec1.size; i++)
        result += vec1.data[i] * vec2.data[i];
    return result;
}
