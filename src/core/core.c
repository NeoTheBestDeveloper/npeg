#include <stdio.h>
#include <string.h>

#include "../utils/utils.h"

#include "./image_formats/image_formats.h"
#include "./image_formats/pbm.h"
#include "./image_formats/pgm.h"

#define PROCESS_IMAGE(image_format)                                            \
    image_format##_image image =                                               \
        create_##image_format##_image(image_file, image_encoding);             \
    fclose(image_file);                                                        \
    process_##image_format##_image(&image, alghoritm);                         \
    dump_##image_format##_image(&image, output_path);                          \
    free_##image_format##_image(&image);

static enum image_formats _parse_image_format(FILE *image_file) {
    char magic_num[3];
    fscanf(image_file, "%2s\n", magic_num);
    fseek(image_file, 0, SEEK_SET);

    if (0 == strcmp(magic_num, "P1") || 0 == strcmp(magic_num, "P4"))
        return PBM;
    else if (0 == strcmp(magic_num, "P2") || 0 == strcmp(magic_num, "P5"))
        return PGM;
    else if (0 == strcmp(magic_num, "P3") || 0 == strcmp(magic_num, "P6"))
        return PPM;
    else if (0 == strcmp(magic_num, "BM"))
        return BMP;
    else
        return UNKNOWN_FORMAT;
}

static enum image_encodings _parse_image_encoding(FILE *image_file) {
    char magic_num[3];
    fscanf(image_file, "%2s\n", magic_num);
    fseek(image_file, 0, SEEK_SET);

    if (0 == strcmp(magic_num, "P1") || 0 == strcmp(magic_num, "P2") ||
        0 == strcmp(magic_num, "P3"))
        return ASCII;
    return RAW;
}

void process_image(const char *input_path, const char *output_path,
                   const char *alghoritm) {
    FILE *image_file = fopen(input_path, "rb");
    enum image_formats image_format = _parse_image_format(image_file);
    enum image_encodings image_encoding = _parse_image_encoding(image_file);

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
        print_error("Error: current input file format is not supported yet.\n",
                    -1);
        break;
    case UNKNOWN_FORMAT:
        print_error("Error: unknown input file format.\n", -1);
        break;
    }
}
