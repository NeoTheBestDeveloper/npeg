#include <stdio.h>
#include <string.h>

#include "../utils/utils.h"
#include "./core.h"

#include "./image_types/pbm_image.h"

#define CONVERT_IMAGE(image_format, image_file, _image_format)                 \
    image_format##_image image =                                               \
        create_##image_format##_image(image_file, _image_format);              \
    convert_##image_format##_image(&image, algorithm);                         \
    dump_image(&image, output_path);                                           \
    free_##image_format##_image(&image);

static enum image_format _parse_image_format(FILE *file) {
    char magic_num[3];
    fscanf(file, "%2s\n", magic_num);

    if (0 == strcmp(magic_num, "P1")) {
        return ASCII_PBM;
    } else if (0 == strcmp(magic_num, "P2")) {
        return ASCII_PGM;
    } else if (0 == strcmp(magic_num, "P3")) {
        return ASCII_PPM;
    } else if (0 == strcmp(magic_num, "P4")) {
        return RAW_PBM;
    } else if (0 == strcmp(magic_num, "P5")) {
        return RAW_PGM;
    } else if (0 == strcmp(magic_num, "P6")) {
        return RAW_PPM;
    } else if (0 == strcmp(magic_num, "BM")) {
        return BMP;
    } else {
        return UNKNOWN_FORMAT;
    }
}

void convert_image(const char *input_path, const char *algorithm,
                   const char *output_path) {
    FILE *image_file = fopen(input_path, "rb");
    enum image_format _image_format = _parse_image_format(image_file);

    switch (_image_format) {
    case RAW_PBM:
    case ASCII_PBM: {
        CONVERT_IMAGE(pbm, image_file, _image_format)
        break;
    }
    case ASCII_PGM:
    case ASCII_PPM:
    case RAW_PGM:
    case RAW_PPM:
    case BMP:
        print_error("Error: current input file format is not supported.\n", -1);
        break;
    case UNKNOWN_FORMAT:
        print_error("Error: unknown input file format.\n", -1);
        break;
    }
}
