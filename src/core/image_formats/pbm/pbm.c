#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../../utils/utils.h"
#include "../image_formats.h"
#include "pbm.h"

#define MAGIC_NUM_LENGTH 2

static void _read_header(pbm_image *image, FILE *image_file) {
    char buffer[255];

    fread(buffer, 1, MAGIC_NUM_LENGTH, image_file);
    buffer[MAGIC_NUM_LENGTH] = 0;
    image->encoding = get_image_encoding(buffer);

    while (1) {
        unsigned char byte = fgetc(image_file);
        if (byte == '#') {
            fgets(buffer, sizeof(buffer), image_file);
        } else if (isdigit(byte)) {
            size_t width, height;
            fseek(image_file, -1, SEEK_CUR);
            fscanf(image_file, "%zu", &width);
            fscanf(image_file, "%zu", &height);

            image->channels[0] = create_matrix(width, height);
            break;
        }
    }
    // skip whitespace symbol
    fgetc(image_file);
}

static void _read_raw_image_data(pbm_image *image, FILE *image_file) {
    for (int i = 0; i < image->channels->height; i++) {
        if (feof(image_file))
            print_error("Error: couldn't read file.", -1);

        unsigned char byte;
        for (int j = 0; j < image->channels->width; j++) {
            if (j % 8 == 0)
                byte = fgetc(image_file);

            // Read byte by bits.
            image->channels[0].data[i][j] = (byte >> (7 - j % 8) & 1);
        }
    }
}

static void _read_ascii_image_data(pbm_image *image, FILE *image_file) {
    char buffer[255];
    for (int i = 0; i < image->channels->height; i++) {
        for (int j = 0; j < image->channels->width; j++) {
            unsigned char byte;
            while (1) {
                if (feof(image_file))
                    print_error("Error: couldn't read file.", -1);

                byte = fgetc(image_file);

                if (byte == '#')
                    fgets(buffer, sizeof(buffer), image_file);
                else if (!isspace(byte))
                    break;
            }

            // Write current byte and convert it from ascii to number.
            image->channels[0].data[i][j] = byte - '0';
        }
    }
}

static void _read_image_data(pbm_image *image, FILE *image_file) {
    if (image->encoding == RAW)
        _read_raw_image_data(image, image_file);
    else
        _read_ascii_image_data(image, image_file);
}

static void _dump_raw_data(pbm_image *image, FILE *fout) {
    for (int i = 0; i < image->channels->height; i++)
        for (int j = 0; j < image->channels->width; j += 8) {
            // Pack every 8 bytes into one byte.
            unsigned char byte = 0;
            for (int k = 0; k < 8; k++)
                byte |= image->channels[0].data[i][j + k] << (7 - k);
            fprintf(fout, "%c", byte);
        }
}

static void _dump_ascii_data(pbm_image *image, FILE *fout) {
    for (int i = 0; i < image->channels->height; i++)
        for (int j = 0; j < image->channels->width; j++)
            fprintf(fout, "%hu", image->channels[0].data[i][j]);
}

pbm_image create_pbm_image(FILE *image_file) {
    pbm_image image = {};
    image.depth = DEPTH;
    image.max_color_value = 1;

    _read_header(&image, image_file);

    _read_image_data(&image, image_file);
    return image;
}

void dump_pbm_image(pbm_image *image, const char *path) {
    char *mode = (image->encoding == RAW) ? "wb" : "wt";
    char *magic_num = (image->encoding == RAW) ? "P4" : "P1";

    FILE *fout = fopen(path, mode);

    // Write magic num.
    fprintf(fout, "%s\n", magic_num);

    // Write image size.
    fprintf(fout, "%zu %zu\n", image->channels->width, image->channels->height);

    if (image->encoding == RAW)
        _dump_raw_data(image, fout);
    else
        _dump_ascii_data(image, fout);

    fclose(fout);
}

void free_pbm_image(pbm_image *image) { free_matrix(image->channels); }
