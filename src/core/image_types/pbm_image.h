#ifndef PBM_IMAGE_H
#define PBM_IMAGE_H

#include "../core.h"
#include <stdio.h>

#define PIXELS_TO_BYTES_WIDTH(width) ceil((double)width / 8.0)

enum direction { VERTICAL, HORIZONTAL };
enum pbm_types { ASCII, RAW };

typedef struct _pbm_image {
    int width, height; // In pixels.
    enum pbm_types type;
    unsigned char **image_data;
} pbm_image;

pbm_image create_pbm_image(FILE *image, enum image_format type);
void dump_image(pbm_image *image, const char *path);
void free_pbm_image(pbm_image *image);
void convert_pbm_image(pbm_image *image, const char *alghoritm);

#endif // PBM_IMAGE_H
