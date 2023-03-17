#include <ctype.h>
#include <fcntl.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../../math/matrix.h"
#include "../img.h"
#include "benchmark.h"
#include "die.h"
#include "pbm.h"

#define SIZE_BUFFER_SIZE (64)
#define BLOCK_SIZE (16 * 1024)

static inline u8 digit_to_chr(u8 digit) { return digit + 48; }
static inline u8 chr_to_digit(u8 digit) { return digit - 48; }

static bool is_ascii(const char *magic) { return 0 == strncmp(magic, "P1", 2); }

static void read_size(i64 *width, i64 *height, i32 fin) {
    char size_buffer[SIZE_BUFFER_SIZE] = {0};
    u8 buffer_top = 0;

    bool width_readen = false;
    bool height_readen = false;

    while (!width_readen && !height_readen) {
        do {
            read(fin, size_buffer + buffer_top, 1);
            buffer_top++;
        } while (!isspace(size_buffer[buffer_top - 1]));
        size_buffer[buffer_top - 1] = '\0';

        *width = atol(size_buffer);
        width_readen = true;
        buffer_top = 0;

        do {
            read(fin, size_buffer + buffer_top, 1);
            buffer_top++;
        } while (!isspace(size_buffer[buffer_top - 1]));
        size_buffer[buffer_top - 1] = '\0';

        *height = atol(size_buffer);
        height_readen = true;
        buffer_top = 0;
    }
}

static void read_data(PbmImg *img, i32 fin) {
    char new_line;
    read(fin, &new_line, 1); // skip whitespace symbol.
    i64 width, height;

    read_size(&width, &height, fin);
    img->img.channels = (Matrix *)malloc(sizeof(Matrix));
    img->img.channels[0] = matrix_new(width, height, U8_MATRIX, false);
    u8 *img_data = (u8 *)img->img.channels->data;

    u8 buf[BLOCK_SIZE];
    i64 buf_size;
    i64 img_data_top = 0;

    while ((buf_size = read(fin, buf, BLOCK_SIZE)) != 0) {
        for (i64 i = 0; i < buf_size; ++i) {
            if (!isspace(buf[i])) {
                img_data[img_data_top++] = chr_to_digit(buf[i]);
            }
        }
    }
}

Img *pbm_img_open(const char *path) {
    PbmImg *new_img = (PbmImg *)malloc(sizeof(PbmImg));

    benchmark(img reading) {
        i32 fin = open(path, O_RDONLY);

        char magic[2];
        read(fin, magic, 2);

        new_img->is_ascii = is_ascii(magic);
        new_img->img.max_colors = 2;
        new_img->img.channels_count = 1;
        new_img->img.format = PBM;

        read_data(new_img, fin);

        close(fin);
    }
    return &new_img->img;
}

static void write_magic(const PbmImg *img, i32 fout) {
    if (img->is_ascii) {
        write(fout, "P1\n", 3);
    } else {
        write(fout, "P4\n", 3);
    }
}

static void write_size(const PbmImg *img, i32 fout) {
    char buf[SIZE_BUFFER_SIZE] = {0};
    sprintf(buf, "%li %li\n", img->img.channels->width,
            img->img.channels->height);
    write(fout, buf, strlen(buf));
}

static void write_data(const PbmImg *img, i32 fout) {
    u8 buf[BLOCK_SIZE];
    u64 buf_top = 0;
    i64 img_size = img->img.channels->height * img->img.channels->width;

    u8 *data = (u8 *)img->img.channels->data;

    for (i64 i = 0; i < img_size; i++) {
        if (buf_top == BLOCK_SIZE) {
            write(fout, buf, buf_top);
            buf_top = 0;
        }
        buf[buf_top++] = digit_to_chr(data[i]);
        buf[buf_top++] = ' ';
    }
    if (buf_top) {
        write(fout, buf, buf_top);
    }
}

void pbm_img_save(const PbmImg *img, i32 fout) {
    write_magic(img, fout);
    write_size(img, fout);
    write_data(img, fout);
}
