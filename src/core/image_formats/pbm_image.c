#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../utils/utils.h"
#include "image_formats.h"
#include "pbm_image.h"

#include "../alghoritms/alghoritms.h"

#define MAGIC_NUM_LENGTH 2

static void _read_header(pbm_image *image, FILE *image_file) {
    char buffer[2048];
    image->comment_lines_count = 0;

    // Skip string with magic num (+ 1 - one whitespace symbol).
    fread(buffer, 1, MAGIC_NUM_LENGTH + 1, image_file);

    // Read comment.
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

    size_t width, height;
    fscanf(image_file, "%zu %zu\n", &width, &height);
    image->channels[0] = create_u8_matrix(width, height);
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
            while (isspace(byte = fgetc(image_file))) {
                if (feof(image_file))
                    print_error("Error: couldn't read file.", -1);
            }
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

void dump_pbm_image(pbm_image *image, const char *path) {
    FILE *fout = fopen(path, "wt");

    // Write magic num.
    fprintf(fout, "%s\n", "P1");

    // Write comments.
    for (int i = 0; i < image->comment_lines_count; i++)
        fprintf(fout, "#%s", image->comments[i]);

    // Write image size.
    fprintf(fout, "%zu %zu\n", image->channels[0].width,
            image->channels[0].height);

    for (int i = 0; i < image->channels[0].height; i++)
        for (int j = 0; j < image->channels[0].width; j++)
            // Before putting pixel at the file convert it from number to ascii.
            fputc(image->channels[0].data[i][j] + '0', fout);

    fclose(fout);
}

pbm_image create_pbm_image(FILE *image_file, enum image_encodings encoding) {
    pbm_image image = {};
    image.encoding = encoding;

    // Read width, height.
    _read_header(&image, image_file);

    _read_image_data(&image, image_file, encoding);
    return image;
}

void free_pbm_image(pbm_image *image) {
    // Free image data.
    free_u8_matrix(image->channels);

    // Free comment.
    for (int i = 0; i < image->comment_lines_count; i++)
        free(image->comments[i]);
    free(image->comments);
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
    }
}
