#include <ctype.h>
#include <stdio.h>

#include "../../../utils/utils.h"
#include "../image_formats.h"
#include "pgm.h"

#define MAGIC_NUM_LENGTH 2

static void _read_header(pgm_image *image, FILE *image_file) {
    char buffer[255];

    fread(buffer, 1, MAGIC_NUM_LENGTH, image_file);
    // Without this string not work with gcc optimizations.
    buffer[MAGIC_NUM_LENGTH] = 0;
    image->encoding = get_image_encoding(buffer);

    volatile int is_size_readen = 0;
    while (1) {
        unsigned char byte = fgetc(image_file);
        if (byte == '#') {
            fgets(buffer, sizeof(buffer), image_file);
        } else if (isdigit(byte) && !is_size_readen) {
            size_t width, height;
            fseek(image_file, -1, SEEK_CUR);
            fscanf(image_file, "%zu", &width);
            fscanf(image_file, "%zu", &height);

            image->channels[0] = create_matrix(width, height);

            is_size_readen = 1;
        } else if (isdigit(byte) && is_size_readen) {
            fseek(image_file, -1, SEEK_CUR);
            fscanf(image_file, "%d", &image->max_color_value);
            break;
        }
    }
    // skip whitespace symbol
    fgetc(image_file);
}

static void _read_raw_image_data(pgm_image *image, FILE *image_file) {
    for (int i = 0; i < image->channels->height; i++)
        for (int j = 0; j < image->channels->width; j++)
            if (image->max_color_value > 255)
                fscanf(image_file, "%d", image->channels[0].data[i] + j);
            else
                fscanf(image_file, "%c", image->channels[0].data[i] + j);
}

static void _read_ascii_image_data(pgm_image *image, FILE *image_file) {
    for (int i = 0; i < image->channels->height; i++)
        for (int j = 0; j < image->channels->width; j++)
            fscanf(image_file, "%d", image->channels[0].data[i] + j);
}

static void _read_image_data(pgm_image *image, FILE *image_file) {
    if (image->encoding == RAW)
        _read_raw_image_data(image, image_file);
    else
        _read_ascii_image_data(image, image_file);
}

static void _dump_raw_data(pgm_image *image, FILE *fout) {
    for (int i = 0; i < image->channels->height; i++)
        for (int j = 0; j < image->channels->width; j++)
            if (image->max_color_value > 255)
                fprintf(fout, "%d", image->channels[0].data[i][j]);
            else
                fprintf(fout, "%c", image->channels[0].data[i][j]);
}

static void _dump_ascii_data(pgm_image *image, FILE *fout) {
    for (int i = 0; i < image->channels->height; i++)
        for (int j = 0; j < image->channels->width; j++) {
            fprintf(fout, "%d ", image->channels[0].data[i][j]);
        }
}

void dump_pgm_image(pgm_image *image, const char *path) {
    char *mode = (image->encoding == RAW) ? "wb" : "wt";
    char *magic_num = (image->encoding == RAW) ? "P5" : "P2";

    FILE *fout = fopen(path, mode);

    // Write magic num.
    fprintf(fout, "%s\n", magic_num);

    // Write image size.
    fprintf(fout, "%zu %zu\n", image->channels[0].width,
            image->channels[0].height);

    // Write max gray value.
    fprintf(fout, "%d\n", image->max_color_value);

    if (image->encoding == RAW)
        _dump_raw_data(image, fout);
    else
        _dump_ascii_data(image, fout);

    fclose(fout);
}

pgm_image create_pgm_image(FILE *image_file) {
    pgm_image image = {};
    image.depth = DEPTH;

    _read_header(&image, image_file);

    _read_image_data(&image, image_file);
    return image;
}

void free_pgm_image(pgm_image *image) { free_matrix(image->channels); }
