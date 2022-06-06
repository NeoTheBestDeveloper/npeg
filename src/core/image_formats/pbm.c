#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../utils/utils.h"
#include "image_formats.h"
#include "pbm.h"

#include "../alghoritms/alghoritms.h"
#include "../alghoritms/filter.h"

#define MAGIC_NUM_LENGTH 2

static void _read_header(pbm_image *image, FILE *image_file) {
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

    image->channels[0] = create_u8_matrix(width, height);

    // Skip whitespace symbol.
    fgetc(image_file);
}

static void _read_raw_image_data(pbm_image *image, FILE *image_file) {
    for (int i = 0; i < image->channels[0].height; i++) {
        if (feof(image_file))
            print_error("Error: couldn't read file.", -1);

        unsigned char byte;
        for (int j = 0; j < image->channels[0].width; j++) {
            if (j % 8 == 0)
                byte = fgetc(image_file);
            // Read byte by bits.
            image->channels[0].data[i][j] = (byte >> (7 - j % 8) & 1);
        }
    }
}

static void _read_ascii_image_data(pbm_image *image, FILE *image_file) {
    for (int i = 0; i < image->channels[0].height; i++) {
        for (int j = 0; j < image->channels[0].width; j++) {
            unsigned char byte;

            // Read bytes, while until read not whitespace symbol.
            while (isspace(byte = fgetc(image_file)))
                if (feof(image_file))
                    print_error("Error: couldn't read file.", -1);

            // Write current byte and convert it from ascii to number.
            image->channels[0].data[i][j] = byte - '0';
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

static void _dump_raw_data(pbm_image *image, FILE *fout) {
    for (int i = 0; i < image->channels[0].height; i++)
        for (int j = 0; j < image->channels[0].width; j += 8) {
            unsigned char byte = 0;
            for (int k = 0; k < 8; k++)
                byte |= image->channels[0].data[i][j + k] << (7 - k);
            fprintf(fout, "%c", byte);
        }
}

static void _dump_ascii_data(pbm_image *image, FILE *fout) {
    for (int i = 0; i < image->channels[0].height; i++)
        for (int j = 0; j < image->channels[0].width; j++)
            // Before putting pixel at the file, convert it from number to
            // ascii.
            fputc(image->channels[0].data[i][j] + '0', fout);
}

void dump_pbm_image(pbm_image *image, const char *path) {
    char *mode = (image->encoding == RAW) ? "wb" : "wt";
    char *magic_num = (image->encoding == RAW) ? "P4" : "P1";

    FILE *fout = fopen(path, mode);

    // Write magic num.
    fprintf(fout, "%s\n", magic_num);

    // Write comments.
    for (int i = 0; i < image->comment_rows_count; i++)
        fprintf(fout, "#%s", image->comment[i]);

    // Write image size.
    fprintf(fout, "%zu %zu\n", image->channels[0].width,
            image->channels[0].height);

    if (image->encoding == RAW)
        _dump_raw_data(image, fout);
    else
        _dump_ascii_data(image, fout);

    fclose(fout);
}

pbm_image create_pbm_image(FILE *image_file, enum image_encodings encoding) {
    pbm_image image = {};
    image.encoding = encoding;

    // Read width, height and comment from header.
    _read_header(&image, image_file);

    _read_image_data(&image, image_file, encoding);
    return image;
}

void free_pbm_image(pbm_image *image) {
    // Free image data.
    free_u8_matrix(image->channels);

    // Free comment.
    for (int i = 0; i < image->comment_rows_count; i++)
        free(image->comment[i]);
    free(image->comment);
}

void process_pbm_image(pbm_image *image, const char *alghoritm) {
    switch (get_alghoritm_type(alghoritm)) {
    case VERTICAL_FLIP:
        flip_u8_matrix(image->channels, VERTICAL);
        break;
    case HORIZONTAL_FLIP:
        flip_u8_matrix(image->channels, HORIZONTAL);
        break;
    case ROTATE_90:
        rotate_u8_matrix(image->channels, 90);
        break;
    case ROTATE_180:
        rotate_u8_matrix(image->channels, 180);
        break;
    case ROTATE_270:
        rotate_u8_matrix(image->channels, 270);
        break;
    case BOX_FILTER_ALGHORITM:
        filter_image(image->channels, 3, BOX_FILTER);
        break;
    case MED_FILTER_ALGHORITM:
        filter_image(image->channels, 3, MED_FILTER);
    case GAUSSIAN_FILTER_ALGHORITM:
        filter_image(image->channels, 3, GAUSSIAN_FILTER);
        break;
    case UNSHARP_FILTER_ALGHORITM:
        filter_image(image->channels, 5, UNSHARP_FILTER);
        break;
    }
}
