#ifndef PBM_IMAGE_H
#define PBM_IMAGE_H

#include <stdio.h>

#include "image_formats.h"

// TODO: add headers.
typedef struct {
    int width, height; // In pixels.
    int bytes_width;
    enum image_encodings encoding;
    unsigned char **image_data;
} pbm_image;

pbm_image create_pbm_image(FILE *image_file, enum image_encodings encoding);
void free_pbm_image(pbm_image *image);

void dump_pbm_image(pbm_image *image, const char *path);
void process_pbm_image(pbm_image *image, const char *alghoritm);

#endif // PBM_IMAGE_H
