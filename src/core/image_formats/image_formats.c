#include <stdio.h>
#include <string.h>

#include "image_formats.h"

enum image_formats get_image_format(FILE *image_file) {
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

enum image_encodings get_image_encoding(const char magic_num[3]) {
    if (0 == strcmp(magic_num, "P1") || 0 == strcmp(magic_num, "P2") ||
        0 == strcmp(magic_num, "P3"))
        return ASCII;
    return RAW;
}
