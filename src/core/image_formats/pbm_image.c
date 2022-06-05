#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../alghoritms/alghoritms.h"
#include "../alghoritms/flip/pbm_flip.h"
#include "image_formats.h"
#include "pbm_image.h"

// TODO: rewrite better.
static void _parse_pbm_header(int image_size[2], FILE *file) {
    char buffer[1024];

    // Skip string with magic num.
    fgets(buffer, sizeof(buffer), file);

    while (1) {
        unsigned char byte = fgetc(file);

        // Skip line if it is comment.
        if (byte == '#') {
            fgets(buffer, sizeof(buffer), file);
        } else {
            fseek(file, -1, SEEK_CUR);
            break;
        }
    }
    fscanf(file, "%d %d\n", image_size, image_size + 1);
}

static unsigned char **_read_image_data(FILE *file, int width, int height,
                                        enum image_encodings encoding) {
    if (encoding == RAW)
        width = PIXELS_TO_BYTES_WIDTH(width);

    unsigned char **image_data =
        (unsigned char **)malloc(sizeof(unsigned char *) * height);

    for (int i = 0; i < height; i++) {
        image_data[i] = (unsigned char *)malloc(width);
        for (int j = 0; j < width; j++) {
            if (encoding == ASCII) {
                unsigned char byte;
                while (1) {
                    fscanf(file, "%c", &byte);
                    if (byte != ' ' && byte != '\n')
                        break;
                }
                image_data[i][j] = byte;
            } else {
                fscanf(file, "%c", image_data[i] + j);
            }
        }
    }

    return image_data;
}

static void _dump_ascii_photo(pbm_image *image, const char *path) {
    FILE *fout = fopen(path, "wt");
    fprintf(fout, "P1\n%d %d\n", image->width, image->height);

    for (int i = 0; i < image->height; i++) {
        for (int j = 0; j < image->width; j++) {
            fprintf(fout, "%c ", image->image_data[i][j]);
        }
    }
    fclose(fout);
}

static void _dump_raw_photo(pbm_image *image, const char *path) {
    FILE *fout = fopen(path, "wb");
    fprintf(fout, "P4\n%d %d\n", image->width, image->height);

    for (int i = 0; i < image->height; i++) {
        for (int j = 0; j < PIXELS_TO_BYTES_WIDTH(image->width); j++) {
            fprintf(fout, "%c", image->image_data[i][j]);
        }
    }
    fclose(fout);
}

pbm_image create_pbm_image(FILE *file, enum image_encodings encoding) {
    int image_size[2]; // 0 - width, 1 - height
    _parse_pbm_header(image_size, file);
    unsigned char **image_data =
        _read_image_data(file, image_size[0], image_size[1], encoding);

    pbm_image image = {image_size[0], image_size[1], encoding, image_data};
    return image;
}

void free_pbm_image(pbm_image *image) {
    for (int i = 0; i < image->height; i++) {
        free(image->image_data[i]);
    }
    free(image->image_data);
}

void dump_pbm_image(pbm_image *image, const char *path) {
    if (image->encoding == RAW)
        _dump_raw_photo(image, path);
    else
        _dump_ascii_photo(image, path);
}

void process_pbm_image(pbm_image *image, const char *alghoritm) {
    switch (get_alghoritm_type(alghoritm)) {
    case VERTICAL_FLIP:
        vertical_pbm_flip(image);
        break;
    case HORIZONTAL_FLIP:
        horizontal_pbm_flip(image);
        break;
    }
}
