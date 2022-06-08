#ifndef PGM_IMAGE_H
#define PGM_IMAGE_H

#include <stddef.h>
#include <stdio.h>

#include "../../math/matrix.h"
#include "../image_formats.h"

#define DEPTH 1

typedef struct {
    size_t width, height;
    union {
        u8_matrix *channels[DEPTH];
        u16_matrix *extended_channels[DEPTH];
    };
    int depth;
    enum image_encodings encoding;
    int max_color_value;
} pgm_image;

pgm_image create_pgm_image(FILE *image_file);
void dump_pgm_image(pgm_image *image, const char *path);
void free_pgm_image(pgm_image *image);

#endif // PGM_IMAGE_H
