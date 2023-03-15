#ifndef H_DIE
#define H_DIE

#include <stdio.h>
#include <stdlib.h>

#ifdef DEBUG
#define die(format, ...)                                                       \
    fprintf(stderr, "Error at the %s:%d.\n", __FILE__, __LINE__);              \
    fprintf(stderr, format, __VA_ARGS__);                                      \
    exit(EXIT_FAILURE);
#endif // DEBUG

#ifndef DEBUG
#define die(format, ...)                                                       \
    fprintf(stderr, format, __VA_ARGS__);                                      \
    exit(EXIT_FAILURE);
#endif // DEBUG

#endif // !H_DIE
