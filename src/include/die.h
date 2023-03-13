#ifndef H_DIE
#define H_DIE

#include <stdio.h>
#include <stdlib.h>

#define die(format, ...)                                                       \
    fprintf(stderr, format, __VA_ARGS__);                                      \
    exit(EXIT_FAILURE);

#endif // !H_DIE
