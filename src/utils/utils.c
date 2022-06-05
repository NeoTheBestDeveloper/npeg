#include <stdio.h>
#include <stdlib.h>

void print_error(const char *msg, int status) {
    // Print error and exit program.
    fprintf(stderr, "%s", msg);
    exit(status);
}
