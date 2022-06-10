#include <stdio.h>
#include <string.h>

#include "../utils/utils.h"

#include "./alghoritms/alghoritms.h"
#include "./image_formats/image_formats.h"
#include "./image_formats/pbm/pbm.h"
#include "./image_formats/pgm/pgm.h"
#include "options.h"

// Polymorphism govna.
#define PROCESS_IMAGE(image_format)                                            \
    image_format##_image image = create_##image_format##_image(image_file);    \
    process_image(image.channels, image.depth, image.max_color_value,          \
                  options);                                                    \
    dump_##image_format##_image(&image, options->output_path);                 \
    free_##image_format##_image(&image);

void handle_image(Options *options) {
    FILE *image_file = fopen(options->input_path, "rb");
    enum image_formats image_format = get_image_format(image_file);

    switch (image_format) {
    case PBM: {
        PROCESS_IMAGE(pbm)
        break;
    }
    case PGM: {
        PROCESS_IMAGE(pgm)
        break;
    }
    case PPM:
    case BMP:
    case PAM:
        print_error("Error: current input file format is not supported yet.\n",
                    -1);
        break;
    case UNKNOWN_FORMAT:
        print_error("Error: unknown input file format.\n", -1);
        break;
    }

    fclose(image_file);
}
