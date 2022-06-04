#include <stdio.h>
#include <stdlib.h>

void print_error(const char *msg, int status_code) {
    fprintf(stderr, msg);
    exit(status_code);
}
