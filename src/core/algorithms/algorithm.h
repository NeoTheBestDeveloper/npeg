#ifndef H_ALGORITHM
#define H_ALGORITHM

#include "types.h"

typedef struct {
    char *key;
    char *value;
} AlgorithmOption;

typedef enum {
    ROTATE,
} AlgorithmName;

typedef struct {
    AlgorithmName name;
    AlgorithmOption *options;
    u32 options_count;
} Algorithm;

Algorithm algorithm_new(const char *name, const char *opts);
void algorithm_free(Algorithm *algorithms);

#endif // !H_ALGORITHM
