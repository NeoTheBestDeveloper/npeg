#ifndef PGM_IMAGE_H
#define PGM_IMAGE_H

#include <stdio.h>

#include "../math/matrix.h"
#include "image_formats.h"

#define CHANNELS_COUNT 1

typedef struct {
    Matrix channels[CHANNELS_COUNT];
    int channels_count;
    enum image_encodings encoding;
    unsigned int max_gray_value;
    char **comment;
    int comment_rows_count;
} pgm_image;

pgm_image create_pgm_image(FILE *image_file, enum image_encodings encoding);
void free_pgm_image(pgm_image *image);

void dump_pgm_image(pgm_image *image, const char *path);

#endif // PGM_IMAGE_H
