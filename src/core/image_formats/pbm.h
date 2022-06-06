#ifndef PBM_IMAGE_H
#define PBM_IMAGE_H

#include <stdio.h>

#include "../math/matrix.h"
#include "image_formats.h"

#define CHANNELS_COUNT 1

typedef struct {
    u8_matrix channels[CHANNELS_COUNT];
    char **comment;
    int comment_rows_count;
    enum image_encodings encoding;
} pbm_image;

pbm_image create_pbm_image(FILE *image_file, enum image_encodings encoding);
void free_pbm_image(pbm_image *image);

void process_pbm_image(pbm_image *image, const char *alghoritm);
void dump_pbm_image(pbm_image *image, const char *path);

#endif // PBM_IMAGE_H
