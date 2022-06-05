#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    image->comment_lines_count = 0;

    // Skip string with magic num (+ 1 - one whitespace symbol).
    fread(buffer, 1, MAGIC_NUM_LENGTH + 1, image_file);

    // Skip string if it is comment.
    while (fgetc(image_file) == '#') {
        fgets(buffer, sizeof(buffer), image_file);
        image->comments =
            (char **)realloc(image->comments, ++(image->comment_lines_count));
        image->comments[image->comment_lines_count - 1] =
            (char *)malloc(strlen(buffer) + 1);
        strncpy(image->comments[image->comment_lines_count - 1], buffer,
                strlen(buffer));

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

    // Write magic num.
    fprintf(fout, "%s\n", magic_num);

    // Write comments.
    for (int i = 0; i < image->comment_lines_count; i++)
        fprintf(fout, "#%s", image->comments[i]);

    // Write image size.
    fprintf(fout, "%d %d\n", image->width, image->height);

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
    // Free image data.
    for (int i = 0; i < image->height; i++)
        free(image->image_data[i]);
    free(image->image_data);

    // Free comment.

    for (int i = 0; i < image->comment_lines_count; i++)
        free(image->comments[i]);
    free(image->comments);
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
