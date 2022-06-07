#ifndef PBM_IMAGE_H
#define PBM_IMAGE_H

#include <stdio.h>

#include "../math/matrix.h"
#include "image_formats.h"

#define CHANNELS_COUNT 1

typedef struct {
    Matrix channels[CHANNELS_COUNT];
    int channels_count;
    enum image_encodings encoding;
    char **comment;
    int comment_rows_count;
} pbm_image;

pbm_image create_pbm_image(FILE *image_file, enum image_encodings encoding);
void free_pbm_image(pbm_image *image);

void dump_pbm_image(pbm_image *image, const char *path);

#endif // PBM_IMAGE_H
