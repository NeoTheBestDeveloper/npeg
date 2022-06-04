#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../alghoritms/alghoritms.h"
#include "../../alghoritms/mirror/pbm_mirror.h"
#include "../core.h"
#include "pbm_image.h"
#include "utils.h"

static void _parse_pbm_header(int image_size[2], FILE *file) {
    fscanf(file, "%d %d\n", image_size, image_size + 1);
}

static unsigned char **_read_image_data(FILE *file, int width, int height,
                                        enum pbm_types type) {
    if (type == RAW)
        width = PIXELS_TO_BYTES_WIDTH(width);

    unsigned char **image_data =
        (unsigned char **)malloc(sizeof(unsigned char *) * height);

    for (int i = 0; i < height; i++) {
        image_data[i] = (unsigned char *)malloc(width);
        for (int j = 0; j < width; j++) {
            fscanf(file, "%c", image_data[i] + j);
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

pbm_image create_pbm_image(FILE *file, enum image_format type) {
    int image_size[2]; // 0 - width, 1 - height
    _parse_pbm_header(image_size, file);
    enum pbm_types image_type = (type == RAW_PBM) ? RAW : ASCII;
    unsigned char **image_data =
        _read_image_data(file, image_size[0], image_size[1], image_type);

    pbm_image image = {image_size[0], image_size[1], image_type, image_data};
    return image;
}

void free_pbm_image(pbm_image *image) {
    for (int i = 0; i < image->height; i++) {
        free(image->image_data[i]);
    }
}

void dump_image(pbm_image *image, const char *path) {
    if (image->type == RAW)
        _dump_raw_photo(image, path);
    else
        _dump_ascii_photo(image, path);
}

void convert_pbm_image(pbm_image *image, const char *alghoritm) {
    enum alghoritms alghoritm_type = get_alghoritm_type(alghoritm);
    switch (alghoritm_type) {
    case VERTICAL_MIRROR:
        vertical_pbm_mirror(image);
        break;
    case HORIZONTAL_MIRROR:
        horizontal_pbm_mirror(image);
        break;
    }
}
