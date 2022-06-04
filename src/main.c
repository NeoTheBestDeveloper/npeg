#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "./core/core.h"
#include "./core/options_validator.h"
#include "./utils/utils.h"

int is_exists_key_option_value(int argc, int key_option_position) {
    return !(key_option_position + 1 >= argc);
}

char *parse_key_option_value(int key_option_position, int argc,
                             const char *key_option_name, char *argv[]) {
    if (is_exists_key_option_value(argc, key_option_position)) {
        return argv[key_option_position + 1];
    }
    fprintf(stderr, "Error: %s key option does not have value\n",
            key_option_name);
    exit(-1);
}

void get_help() {
    printf("Usage:\n\tnpeg -i <INPUT_PATH> -a <ALGORITHM_NAME> -o "
           "<OUTPUT_PATH>\n\nAviable formats:\n\tPBM\n\nAviable "
           "algorithms:\n\tvertical_mirror\t\tMirroring image by y "
           "axe.\n\thorizontal_mirror\tMirroring image by x axe.\n");
    exit(0);
}

int main(int argc, char *argv[]) {
    // Check arguments exsisting.
    if (argc == 1) {
        print_error("Error: no arguments\n", -1);
    }

    // Show help message.
    if (0 == strcmp(argv[1], "-h") || 0 == strcmp(argv[1], "--help")) {
        get_help();
    }

    char *input_path = NULL;
    char *output_path = NULL;
    char *algorithm = NULL;

    // Parse options.
    for (int i = 1; i < argc; i++) {
        if (0 == strcmp(argv[i], "-i") || 0 == strcmp(argv[i], "--input")) {
            input_path = parse_key_option_value(i, argc, "input", argv);
            i++;
        } else if (0 == strcmp(argv[i], "-o") ||
                   0 == strcmp(argv[i], "--output")) {
            output_path = parse_key_option_value(i, argc, "output", argv);
            i++;
        } else if (0 == strcmp(argv[i], "-a") ||
                   0 == strcmp(argv[i], "--algorithm")) {
            algorithm = parse_key_option_value(i, argc, "algorithm", argv);
            i++;
        } else {
            fprintf(stderr, "Error: unknown option \"%s\".\n", argv[i]);
            exit(-1);
        }
    }

    validate_options_value(input_path, output_path, algorithm);

    // Convert input image.
    convert_image(input_path, output_path, algorithm);

    return 0;
}
