#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../math/matrix/matrix.h"
#include "die.h"
#include "img.h"
#include "img_magics.h"
#include "pbm/pbm.h"

static bool array_cmp(const u8 *arr1, const u8 *arr2, u64 len) {
    for (u64 i = 0; i < len; i++) {
        if (arr1[i] != arr2[i])
            return false;
    }
    return true;
}

static bool is_png(i32 fin) {
    u8 buf[8];
    read(fin, buf, 8);
    lseek(fin, 0, SEEK_SET);

    return array_cmp(png_magic, buf, 8);
}

static bool is_jpg(i32 fin) {
    u8 buf[3];
    read(fin, buf, 3);
    lseek(fin, 0, SEEK_SET);

    return array_cmp(jpg_magic, buf, 3);
}

static const char *format_to_text(ImageFormat format) {
    switch (format) {
    case PNG:
        return "PNG";
    case JPG:
        return "JPG";
    case PBM:
        return "PBM";
    case PGM:
        return "PGM";
    case PAM:
        return "PAM";
    case PPM:
        return "PPM";
    default:
        return "unknown";
    }
}
ImageFormat guess_image_format(i32 fin) {
    if (is_png(fin)) {
        return PNG;
    }
    if (is_jpg(fin)) {
        return JPG;
    }

    char buf[3] = {0};
    read(fin, buf, 2);
    lseek(fin, 0, SEEK_SET);

    if (0 == strncmp(buf, PBM_MAGIC, 2) ||
        0 == strncmp(buf, ASCII_PBM_MAGIC, 2)) {
        return PBM;
    }
    if (0 == strncmp(buf, PGM_MAGIC, 2) ||
        0 == strncmp(buf, ASCII_PGM_MAGIC, 2)) {
        return PGM;
    }
    if (0 == strncmp(buf, PPM_MAGIC, 2) ||
        0 == strncmp(buf, ASCII_PPM_MAGIC, 2)) {
        return PPM;
    }

    if (0 == strncmp(buf, PAM_MAGIC, 2)) {
        return PAM;
    }

    die("Error: unknown image format.\n", NULL)
}

Img *img_open(const char *path) {
    i32 fin = open(path, O_RDONLY);
    ImageFormat format = guess_image_format(fin);
    close(fin);

    switch (format) {
    case PBM:
        return pbm_img_open(path);
    case PNG:
    case JPG:
    case PGM:
    case PAM:
    case PPM:
        die("Error: this image format (%s) is not supported yet.\n",
            format_to_text(format));
    default:
        die("Error: unknown image format.\n", NULL)
    }
}

static void _img_free(Img *img) {
    for (u8 i = 0; i < img->channels_count; i++) {
        matrix_free(img->channels + i);
    }

    free(img->channels);
    free(img);
}

void img_free(Img *img) {
    switch (img->format) {
    case PBM:
        pbm_img_free((PbmImg *)img);
        break;
    case PNG:
    case JPG:
    case PGM:
    case PAM:
    case PPM:
        die("Error: this image format (%s) is not supported yet.\n",
            format_to_text(img->format));
    }
    _img_free(img);
}

void img_rotate(Img *img, f32 degrees) {
    for (u8 i = 0; i < img->channels_count; i++) {
        matrix_rotate(img->channels + i, degrees);
    }
}

void img_save(const Img *img, const char *path) {
    switch (img->format) {
    case PBM:
        pbm_img_save((const PbmImg *)img, path);
        break;
    case PNG:
    case JPG:
    case PGM:
    case PAM:
    case PPM:
        die("Error: this image format (%s) is not supported yet.\n",
            format_to_text(img->format));
    }
}