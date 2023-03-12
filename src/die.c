#include <die.h>

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

noreturn void die(const char *msg) {
    write(STDERR_FILENO, msg, strlen(msg));
    exit(EXIT_FAILURE);
}
