#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../utils/utils.h"
#include "image_formats.h"
#include "pgm.h"

#include "../alghoritms/alghoritms.h"
#include "../alghoritms/filter.h"

#define MAGIC_NUM_LENGTH 2

static void _read_header(pgm_image *image, FILE *image_file) {
    char buffer[2048];
    image->comment_rows_count = 0;

    // Skip string with magic num (+ 1 - one whitespace symbol).
    fread(buffer, 1, MAGIC_NUM_LENGTH + 1, image_file);

    // Read comment.
    while (fgetc(image_file) == '#') {
        fgets(buffer, sizeof(buffer), image_file);
        image->comment =
            (char **)realloc(image->comment, ++(image->comment_rows_count));
        image->comment[image->comment_rows_count - 1] =
            (char *)malloc(strlen(buffer) + 1);
        strncpy(image->comment[image->comment_rows_count - 1], buffer,
                strlen(buffer));

        if (feof(image_file))
            print_error("Error: couldn't read file.", -1);
    }

    // Move cursor back, because we read one byte of image width before.
    fseek(image_file, -1, SEEK_CUR);

    size_t width, height;
    fscanf(image_file, "%zu", &width);

    // Skip whitespace symbol.
    fgetc(image_file);

    fscanf(image_file, "%zu", &height);
    image->channels[0] = create_matrix(width, height);

    // Skip whitespace symbol.
    fgetc(image_file);

    fscanf(image_file, "%ul", &image->max_gray_value);

    // Skip whitespace symbol.
    fgetc(image_file);
}

static void _read_raw_image_data(pgm_image *image, FILE *image_file) {
    for (int i = 0; i < image->channels[0].height; i++) {
        if (feof(image_file))
            print_error("Error: couldn't read file.", -1);
        for (int j = 0; j < image->channels[0].width; j++) {
            if (image->max_gray_value > 255)
                fscanf(image_file, "%lu", image->channels[0].data[i] + j);
            else
                fscanf(image_file, "%c", image->channels[0].data[i] + j);
        }
    }
}

static void _read_ascii_image_data(pgm_image *image, FILE *image_file) {
    for (int i = 0; i < image->channels[0].height; i++) {
        for (int j = 0; j < image->channels[0].width; j++) {
            if (feof(image_file))
                print_error("Error: couldn't read file.", -1);
            fscanf(image_file, "%lu", image->channels[0].data[i] + j);
        }
    }
}

static void _read_image_data(pgm_image *image, FILE *image_file,
                             enum image_encodings encoding) {
    if (encoding == RAW)
        _read_raw_image_data(image, image_file);
    else
        _read_ascii_image_data(image, image_file);
}

static void _dump_raw_data(pgm_image *image, FILE *fout) {
    for (int i = 0; i < image->channels[0].height; i++)
        for (int j = 0; j < image->channels[0].width; j++) {
            if (image->max_gray_value > 255)
                fprintf(fout, "%lu", image->channels[0].data[i][j]);
            else
                fprintf(fout, "%c", image->channels[0].data[i][j]);
        }
}

static void _dump_ascii_data(pgm_image *image, FILE *fout) {
    for (int i = 0; i < image->channels[0].height; i++)
        for (int j = 0; j < image->channels[0].width; j++)
            fprintf(fout, "%lu ", image->channels[0].data[i][j]);
}

void dump_pgm_image(pgm_image *image, const char *path) {
    char *mode = (image->encoding == RAW) ? "wb" : "wt";
    char *magic_num = (image->encoding == RAW) ? "P5" : "P2";

    FILE *fout = fopen(path, mode);

    // Write magic num.
    fprintf(fout, "%s\n", magic_num);

    // Write comments.
    for (int i = 0; i < image->comment_rows_count; i++)
        fprintf(fout, "#%s", image->comment[i]);

    // Write image size.
    fprintf(fout, "%zu %zu\n", image->channels[0].width,
            image->channels[0].height);

    // Write max gray value.
    fprintf(fout, "%d\n", image->max_gray_value);

    if (image->encoding == RAW)
        _dump_raw_data(image, fout);
    else
        _dump_ascii_data(image, fout);

    fclose(fout);
}

pgm_image create_pgm_image(FILE *image_file, enum image_encodings encoding) {
    pgm_image image = {};
    image.encoding = encoding;
    image.channels_count = CHANNELS_COUNT;

    // Read width, height and comment from header.
    _read_header(&image, image_file);

    _read_image_data(&image, image_file, encoding);
    return image;
}

void free_pgm_image(pgm_image *image) {
    // Free image data.
    free_matrix(image->channels);

    // Free comment.
    for (int i = 0; i < image->comment_rows_count; i++)
        free(image->comment[i]);
    free(image->comment);
}
