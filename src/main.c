#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./core/core.h"
#include "./utils/utils.h"
#include "./validator/options_validator.h"

#define VERSION "0.0.1"

int is_exists_key_option_value(int argc, int key_option_position) {
    return !(key_option_position + 1 >= argc);
}

char *parse_key_option_value(int key_option_position, int argc,
                             const char *key_option_name, char *argv[]) {
    // Check option value exsisting and return it if ok.
    if (is_exists_key_option_value(argc, key_option_position)) {
        return argv[key_option_position + 1];
    }
    fprintf(stderr, "Error: \"%s\" key option does not have value\n",
            key_option_name);
    exit(-1);
}

void print_help() {
    printf("npeg %s\n"
           "A command-line bitmap images processor.\n\n"
           "Usage:\n\tnpeg -i <INPUT_PATH> -a <ALGHORITM> -o "
           "<OUTPUT_PATH>\n\n"
           "Options:\n\t"
           "-a, --alghoritm <ALGHORITM>\t\tAlghoritm name to process image.\n\t"
           "-i, --input <INPUT_PATH>\t\tPath to input image.\n\t"
           "-o, --output <OUTPUT_PATH>\t\tPath to output image.\n\t"
           "-h, --help\t\t\t\tPrint help information.\n\n"
           "Aviable formats:\n\tPBM\n\n"
           "Aviable algorithms:\n\t"
           "vertical_flip\t\tFlip image by y axe.\n\t"
           "horizontal_mirror\tFlip image by x axe.\n\t"
           "rotate90\t\tRotate image by 90 degrees.\n\t"
           "rotate180\t\tRotate image by 180 degrees.\n\t"
           "rotate270\t\tRotate image by 270 degrees.\n\n"
           "Author:\n\tnpeg was written by Neo <kiankasey91@gmail.com>.\n",
           VERSION);
    exit(0);
}

int main(int argc, char *argv[]) {
    // Check options exsisting.
    if (argc == 1) {
        print_error("Error: no options.\n", -1);
    }

    // Show help message.
    if (0 == strcmp(argv[1], "-h") || 0 == strcmp(argv[1], "--help")) {
        print_help();
    }

    char *input_path = NULL;
    char *output_path = NULL;
    char *alghoritm = NULL;

    // Parse options.
    for (int i = 1; i < argc; i++) {
        // Parse <INPUT_PATH>.
        if (0 == strcmp(argv[i], "-i") || 0 == strcmp(argv[i], "--input")) {
            input_path = parse_key_option_value(i, argc, "input", argv);
            i++;
        }
        // Parse <OUTPUT_PATH>.
        else if (0 == strcmp(argv[i], "-o") ||
                 0 == strcmp(argv[i], "--output")) {
            output_path = parse_key_option_value(i, argc, "output", argv);
            i++;
        }
        // Parse <ALGHORITM>.
        else if (0 == strcmp(argv[i], "-a") ||
                 0 == strcmp(argv[i], "--alghoritm")) {
            alghoritm = parse_key_option_value(i, argc, "alghoritm", argv);
            i++;
        } else {
            fprintf(stderr, "Error: unknown option \"%s\".\n", argv[i]);
            exit(-1);
        }
    }

    validate_options_value(input_path, output_path, alghoritm);

    // Process input image.
    process_image(input_path, output_path, alghoritm);

    return 0;
}
