#ifndef PBM_IMAGE_H
#define PBM_IMAGE_H

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
} pbm_image;

pbm_image create_pbm_image(FILE *image_file);
void dump_pbm_image(pbm_image *image, const char *path);
void free_pbm_image(pbm_image *image);

#endif // PBM_IMAGE_H
