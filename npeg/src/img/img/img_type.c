#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#define LEN(arr) ((sizeof arr) / (sizeof arr[0]))

#include "img_type.h"
#include "magic.h"

static bool array_cmp(const u8 *arr1, const u8 *arr2, u64 len) {
    for (u64 i = 0; i < len; i++) {
        if (arr1[i] != arr2[i])
            return false;
    }
    return true;
}

static bool is_png(i32 fin) {
    u8 buf[LEN(png_magic)];

    read(fin, buf, LEN(png_magic));
    lseek(fin, 0, SEEK_SET);

    return array_cmp(png_magic, buf, LEN(png_magic));
}

static bool is_webp(i32 fin) {
    u8 buf1[LEN(webp_magic_first_half)];
    u8 buf2[LEN(webp_magic_second_half)];

    read(fin, buf1, LEN(webp_magic_first_half));
    lseek(fin, 4, SEEK_CUR);

    read(fin, buf2, LEN(webp_magic_second_half));
    lseek(fin, 0, SEEK_SET);

    return array_cmp(webp_magic_first_half, buf1, LEN(webp_magic_first_half)) ||
           array_cmp(webp_magic_second_half, buf2, LEN(webp_magic_second_half));
}

static bool is_tiff(i32 fin) {
    u8 buf[LEN(tiff_magic_be)];

    read(fin, buf, LEN(tiff_magic_be));
    lseek(fin, 0, SEEK_SET);

    return array_cmp(tiff_magic_be, buf, LEN(tiff_magic_be)) ||
           array_cmp(tiff_magic_le, buf, LEN(tiff_magic_le));
}

static bool is_jpg(i32 fin) {
    u8 buf[LEN(jpg_magic)];

    read(fin, buf, LEN(jpg_magic));
    lseek(fin, 0, SEEK_SET);

    return array_cmp(jpg_magic, buf, LEN(jpg_magic));
}

const char *img_type_to_str(ImgType type) {
    switch (type) {
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
    case WEBP:
        return "WEBP";
    case TIFF:
        return "TIFF";
    case UNKNOWN_MAGIC:
        return "unknown";
    default:
        return "unknown";
    }
}

ImgType guess_img_type(i32 fin) {
    if (is_png(fin)) {
        return PNG;
    } else if (is_jpg(fin)) {
        return JPG;
    } else if (is_tiff(fin)) {
        return TIFF;
    } else if (is_webp(fin)) {
        return WEBP;
    }

    char buf[2];
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

    return UNKNOWN_MAGIC;
}
