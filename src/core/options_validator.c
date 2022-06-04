#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "../alghoritms/alghoritms.h"
#include "../utils/utils.h"
#include "options_validator.h"

static void _validate_input_path(const char *input_path) {
    if (input_path == NULL)
        print_error("Error: no input path.\n", -1);

    if (access(input_path, F_OK) != 0)
        print_error("Error: input file path does not exists.\n", -1);
    else if (access(input_path, R_OK) != 0)
        print_error("Error: does not has permisions to read input file.\n", -1);
}

static void _validate_output_path(const char *output_path) {
    if (output_path == NULL)
        print_error("Error: no output path.\n", -1);
}

static void _validate_algorithm(const char *algoritm) {
    if (algoritm == NULL)
        print_error("Error: no alghoritm name.\n", -1);

    if (get_alghoritm_type(algoritm) == -1)
        print_error("Error: unknown alghoritm.\n", -1);
}

void validate_options_value(const char *input_path, const char *output_path,
                            const char *alghorithm) {
    _validate_input_path(input_path);
    _validate_output_path(output_path);
    _validate_algorithm(alghorithm);
}
