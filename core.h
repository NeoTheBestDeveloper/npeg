#include <stdio.h>

enum image_format { PBM, PGM, PPM, BMP, UNKNOWN_FORMAT };
enum image_encoding { ASCII, RAW, UNKNOWN_ENCODING };

image_format parse_image_format(const FILE *image);
image_encoding parse_image_encoding(const FILE *image);
