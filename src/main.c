#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./core/core.h"
#include "./utils/utils.h"
#include "./validator/options_validator.h"
#include "core/options.h"

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
           "rotate270\t\tRotate image by 270 degrees.\n\t"
           "box_filter\t\tFilter image with box filter.\n\t"
           "median_filter\t\tFilter image with median filter.\n\t"
           "gaussian_filter\t\tFilter image with gaussian filter.\n\t"
           "unsharp_filter\t\tFilter image with unsharp filter.\n\n"
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

    Options options = {
        .input_path = NULL,
        .output_path = NULL,
        .alghoritm = NULL,
        .filter_size = "3",
        .sigma = "1.0",
    };

    // Parse options.
    for (int i = 1; i < argc; i++) {
        // Parse <INPUT_PATH>.
        if (0 == strcmp(argv[i], "-i") || 0 == strcmp(argv[i], "--input")) {
            options.input_path = parse_key_option_value(i, argc, "input", argv);
            i++;
        }
        // Parse <OUTPUT_PATH>.
        else if (0 == strcmp(argv[i], "-o") ||
                 0 == strcmp(argv[i], "--output")) {
            options.output_path =
                parse_key_option_value(i, argc, "output", argv);
            i++;
        }
        // Parse <ALGHORITM>.
        else if (0 == strcmp(argv[i], "-a") ||
                 0 == strcmp(argv[i], "--alghoritm")) {
            options.alghoritm =
                parse_key_option_value(i, argc, "alghoritm", argv);
            i++;
        }
        // Parse <FILTER_SIZE>.
        else if (0 == strcmp(argv[i], "-f") ||
                 0 == strcmp(argv[i], "--filter_size")) {
            options.filter_size =
                parse_key_option_value(i, argc, "filter size", argv);
            i++;
        }
        // Parse <SIGMA>.
        else if (0 == strcmp(argv[i], "-s") ||
                 0 == strcmp(argv[i], "--sigma")) {
            options.sigma = parse_key_option_value(i, argc, "sigma", argv);
            i++;
        } else {
            fprintf(stderr, "Error: unknown option \"%s\".\n", argv[i]);
            exit(-1);
        }
    }

    validate_options(&options);

    // Process input image.
    handle_image(&options);

    return 0;
}
