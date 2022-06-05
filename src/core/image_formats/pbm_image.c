#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../utils/utils.h"
#include "../alghoritms/alghoritms.h"
#include "../alghoritms/flip/pbm_flip.h"
#include "image_formats.h"
#include "pbm_image.h"

#define MAGIC_NUM_LENGTH 2

static int _pixels_to_bytes_width(int width) {
    return ceil((double)width / 8.0);
}

static void _read_header(pbm_image *image, FILE *image_file) {
    char buffer[2048];

    // Skip string with magic num (+ 1 - one whitespace symbol).
    fread(buffer, 1, MAGIC_NUM_LENGTH + 1, image_file);

    // Skip string if it is comment.
    while (fgetc(image_file) == '#') {
        fgets(buffer, sizeof(buffer), image_file);

        if (feof(image_file))
            print_error("Error: couldn't read file.", -1);
    }
    fseek(image_file, -1, SEEK_CUR);

    fscanf(image_file, "%d%d", &image->width, &image->height);
    image->bytes_width = (image->encoding == RAW)
                             ? _pixels_to_bytes_width(image->width)
                             : image->width;
}

static void _read_raw_image_data(pbm_image *image, FILE *image_file) {
    image->image_data =
        (unsigned char **)malloc(sizeof(unsigned char *) * image->height);
    for (int i = 0; i < image->height; i++) {
        if (feof(image_file))
            print_error("Error: couldn't read file.", -1);

        image->image_data[i] = (unsigned char *)malloc(image->bytes_width);
        fread(image->image_data[i], 1, image->bytes_width, image_file);
    }
}

static void _read_ascii_image_data(pbm_image *image, FILE *image_file) {
    image->image_data =
        (unsigned char **)malloc(sizeof(unsigned char *) * image->height);
    for (int i = 0; i < image->height; i++) {
        image->image_data[i] = (unsigned char *)malloc(image->bytes_width);
        for (int j = 0; j < image->bytes_width; j++) {
            unsigned char byte;
            while (isspace(byte = fgetc(image_file))) {
                if (feof(image_file))
                    print_error("Error: couldn't read file.", -1);
            }
            image->image_data[i][j] = byte;
        }
    }
}

static void _read_image_data(pbm_image *image, FILE *image_file,
                             enum image_encodings encoding) {
    if (encoding == RAW)
        _read_raw_image_data(image, image_file);
    else
        _read_ascii_image_data(image, image_file);
}

void dump_pbm_image(pbm_image *image, const char *path) {
    char *mode = (image->encoding == RAW) ? "wb" : "wt";
    char *magic_num = (image->encoding == RAW) ? "P4" : "P1";

    FILE *fout = fopen(path, mode);
    fprintf(fout, "%s\n%d %d\n", magic_num, image->width, image->height);

    for (int i = 0; i < image->height; i++)
        fwrite(image->image_data[i], 1, image->bytes_width, fout);

    fclose(fout);
}

pbm_image create_pbm_image(FILE *image_file, enum image_encodings encoding) {
    pbm_image image = {
        .encoding = encoding,
    };

    // Read width, height, calc bytes width.
    _read_header(&image, image_file);

    _read_image_data(&image, image_file, encoding);
    return image;
}

void free_pbm_image(pbm_image *image) {
    for (int i = 0; i < image->height; i++)
        free(image->image_data[i]);
    free(image->image_data);
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
