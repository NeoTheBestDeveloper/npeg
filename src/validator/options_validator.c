#include <stdio.h>
#include <unistd.h>

#include "../core/alghoritms/alghoritms.h"
#include "../utils/utils.h"

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

static void _validate_alghoritm(const char *alghoritm) {
    if (alghoritm == NULL)
        print_error("Error: no alghoritm name.\n", -1);

    if (get_alghoritm_type(alghoritm) == -1)
        print_error("Error: unknown alghoritm.\n", -1);
}

static void _validate_sigma(char *sigma) {
    float buffer;
    if (sscanf(sigma, "%f", &buffer) != 1)
        print_error("Error: sigma must be a number.\n", -1);
    if (buffer < 0.0)
        print_error("Error: sigma must be greater then zero.\n", -1);
}

static void _validate_filter_size(char *filter_size) {
    int buffer;
    if (sscanf(filter_size, "%d", &buffer) != 1)
        print_error("Error: filter size must be a number.\n", -1);
    if (buffer < 3)
        print_error("Error: filter size must be greater-equal then 3.\n", -1);
    if (buffer % 2 == 0)
        print_error("Error: filter size must be odd.\n", -1);
}

void validate_options(Options *options) {
    _validate_input_path(options->input_path);
    _validate_output_path(options->output_path);
    _validate_alghoritm(options->alghoritm);
    _validate_sigma(options->sigma);
    _validate_filter_size(options->filter_size);
}
