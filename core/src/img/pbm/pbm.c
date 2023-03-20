#include <ctype.h>
#include <fcntl.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../../math/matrix.h"
#include "benchmark.h"
#include "pbm.h"

#define IMG_HEADER_BUFFER_SIZE (64)
#define BLOCK_SIZE (16 * 1024)

static inline u8 digit_to_chr(u8 digit) { return digit + 48; }
static inline u8 chr_to_digit(u8 digit) { return digit - 48; }

static bool is_ascii(const char *magic) { return 0 == strncmp(magic, "P1", 2); }

static void read_size(i64 *width, i64 *height, i32 fin) {
    char header_buf[IMG_HEADER_BUFFER_SIZE] = {0};
    u8 buffer_top = 0;

    bool width_readen = false;
    bool height_readen = false;

    while (!width_readen && !height_readen) {
        do {
            read(fin, header_buf + buffer_top, 1);
            buffer_top++;
        } while (!isspace(header_buf[buffer_top - 1]));
        header_buf[buffer_top - 1] = '\0';

        *width = atol(header_buf);
        width_readen = true;
        buffer_top = 0;

        do {
            read(fin, header_buf + buffer_top, 1);
            buffer_top++;
        } while (!isspace(header_buf[buffer_top - 1]));
        header_buf[buffer_top - 1] = '\0';

        *height = atol(header_buf);
        height_readen = true;
        buffer_top = 0;
    }
}

static void read_data(PbmImg *img, i32 fin) {
    i64 width, height;

    read_size(&width, &height, fin);

    img->img.channels = (Matrix *)malloc(sizeof *img->img.channels);
    img->img.channels[0] = matrix_new(width, height, U8_MATRIX, false);

    u8 *img_data = (u8 *)img->img.channels[0].data;

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

Img *pbm_img_read(const char *path) {
    PbmImg *new_img = (PbmImg *)malloc(sizeof *new_img);

    i32 fin = open(path, O_RDONLY);

    char magic[3];
    read(fin, magic, 3);

    new_img->is_ascii = is_ascii(magic);
    new_img->img.depth = 1;
    new_img->img.channels_count = 1;
    new_img->img.type = PBM;

    read_data(new_img, fin);
    close(fin);

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
    char header_buf[IMG_HEADER_BUFFER_SIZE] = {0};
    sprintf(header_buf, "%li %li\n", img->img.channels->width,
            img->img.channels->height);
    write(fout, header_buf, strlen(header_buf));
}

static void write_data(const PbmImg *img, i32 fout) {
    u8 buf[BLOCK_SIZE];
    u64 buf_top = 0;
    i64 img_size = img->img.channels[0].height * img->img.channels[0].width;

    u8 *data = (u8 *)img->img.channels->data;

    for (i64 i = 0; i < img_size; ++i) {
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

void pbm_img_write(const PbmImg *img, i32 fout) {
    write_magic(img, fout);
    write_size(img, fout);
    write_data(img, fout);
}

Img *pbm_img_copy(const PbmImg *img) {
    PbmImg *new_img = (PbmImg *)malloc(sizeof *new_img);
    memcpy(new_img, img, sizeof *new_img);

    new_img->img.channels = (Matrix *)malloc(sizeof *img->img.channels);
    new_img->img.channels[0] = matrix_copy(img->img.channels + 0);

    return &new_img->img;
}
