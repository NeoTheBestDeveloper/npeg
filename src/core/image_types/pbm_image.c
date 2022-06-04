#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../core.h"
#include "pbm_image.h"
#include "utils.h"

static void _parse_pbm_header(int image_size[2], FILE *file) {
    fscanf(file, "%d %d\n", image_size, image_size + 1);
}

static unsigned char _reverse_bits(unsigned char n) {
    unsigned int NO_OF_BITS = sizeof(n) * 8;
    unsigned char reverse_num = 0;
    for (int i = 0; i < NO_OF_BITS; i++) {
        if ((n & (1 << i)))
            reverse_num |= 1 << ((NO_OF_BITS - 1) - i);
    }
    return reverse_num;
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

static void _vertical_reverse_ascii_photo(pbm_image *image) {
    for (int i = 0; i < image->height; i++) {
        for (int j = 0; j < image->width / 2; j++) {
            unsigned char tmp = image->image_data[i][j];
            image->image_data[i][j] =
                image->image_data[i][image->width - j - 1];
            image->image_data[i][image->width - j - 1] = tmp;
        }
    }
}

static void _vertical_reverse_raw_photo(pbm_image *image) {
    int width = PIXELS_TO_BYTES_WIDTH(image->width);
    for (int i = 0; i < image->height; i++) {
        for (int j = 0; j < width / 2; j++) {
            unsigned char tmp = image->image_data[i][j];
            image->image_data[i][j] =
                _reverse_bits(image->image_data[i][width - j - 1]);
            image->image_data[i][width - j - 1] = _reverse_bits(tmp);
        }
    }
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

void reverse_photo(pbm_image *image, enum direction _direction) {
    switch (_direction) {
    case VERTICAL:
        if (image->type == RAW)
            _vertical_reverse_raw_photo(image);
        else
            _vertical_reverse_ascii_photo(image);
        break;
        // case HORIZONTAL:
        //     if (image->type == RAW)
        //         _horizontal_reverse_raw_photo(image);
        //     else
        //         _horizontal_reverse_ascii_photo(image);
        //     break;
    }
}
void convert_pbm_image(pbm_image *image, const char *alghoritm) {}
