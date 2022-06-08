#ifndef IMAGE_FORMATS_H
#define IMAGE_FORMATS_H

#include <stdio.h>

enum image_formats { PGM, PAM, PPM, PBM, BMP, UNKNOWN_FORMAT };
enum image_encodings { RAW, ASCII };

enum image_encodings get_image_encoding(const char magic_num[3]);
enum image_formats get_image_format(FILE *image_file);

#endif // IMAGE_FORMATS_H
