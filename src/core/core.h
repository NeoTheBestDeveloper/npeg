#ifndef CORE_H
#define CORE_H

#include <stdio.h>

enum image_format {
    ASCII_PBM,
    ASCII_PGM,
    ASCII_PPM,
    RAW_PBM,
    RAW_PGM,
    RAW_PPM,
    BMP,
    UNKNOWN_FORMAT
};

void convert_image(const char *input_path, const char *output_path,
                   const char *alghoritm);

#endif
