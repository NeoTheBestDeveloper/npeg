#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "../../utils/utils.h"
#include "image_formats.h"
#include "pbm_image.h"

#include "../alghoritms/alghoritms.h"
#include "../alghoritms/flip/pbm_flip.h"
#include "../alghoritms/rotate/pbm_rotate.h"

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

    fscanf(image_file, "%d %d\n", &image->width, &image->height);
}

static void _read_raw_image_data(pbm_image *image, FILE *image_file) {
    image->image_data =
        (unsigned char **)malloc(sizeof(unsigned char *) * image->height);
    for (int i = 0; i < image->height; i++) {
        if (feof(image_file))
            print_error("Error: couldn't read file.", -1);

        image->image_data[i] = (unsigned char *)malloc(image->width);
        unsigned char byte;
        for (int j = 0; j < image->width; j++) {
            if (j % 8 == 0)
                byte = fgetc(image_file);
            image->image_data[i][j] = (byte >> (7 - j % 8) & 1) + '0';
        }
    }
}

static void _read_ascii_image_data(pbm_image *image, FILE *image_file) {
    image->image_data =
        (unsigned char **)malloc(sizeof(unsigned char *) * image->height);
    for (int i = 0; i < image->height; i++) {
        image->image_data[i] = (unsigned char *)malloc(image->width);
        for (int j = 0; j < image->width; j++) {
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
    FILE *fout = fopen(path, "wt");

    // Write magic num.
    fprintf(fout, "%s\n", "P1");

    // Write comments.
    for (int i = 0; i < image->comment_lines_count; i++)
        fprintf(fout, "#%s", image->comments[i]);

    // Write image size.
    fprintf(fout, "%d %d\n", image->width, image->height);

    for (int i = 0; i < image->height; i++)
        fwrite(image->image_data[i], 1, image->width, fout);

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
        // case ROTATE_90:
        //     pbm_rotate(image, 90);
        //     break;
        // case ROTATE_180:
        //     pbm_rotate(image, 180);
        //     break;
        // case ROTATE_270:
        //     pbm_rotate(image, 270);
        //     break;
    }
}
