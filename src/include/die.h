#ifndef H_DIE
#define H_DIE

#include <stdio.h>
#include <stdlib.h>

#ifdef DEBUG
#define die(format, ...)                                                       \
    fprintf(stderr, "\033[38;5;9mError at the %s:%d.\n", __FILE__, __LINE__);  \
    fprintf(stderr, format, __VA_ARGS__);                                      \
    fprintf(stderr, "\033[m");                                                 \
    exit(EXIT_FAILURE);
#endif // DEBUG

#ifndef DEBUG
#define die(format, ...)                                                       \
    fprintf(stderr, "\033[38;5;9m", __FILE__, __LINE__);                       \
    fprintf(stderr, format, __VA_ARGS__);                                      \
    fprintf(stderr, "\033[m");                                                 \
    exit(EXIT_FAILURE);
#endif // DEBUG

#endif // !H_DIE
