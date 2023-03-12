#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <types.h>
#include <unistd.h>

#include "core.h"
#include "die.h"

#define ASCII_PBM_MAGIC "P1"
#define ASCII_PGM_MAGIC "P2"
#define ASCII_PPM_MAGIC "P3"

#define PBM_MAGIC "P4"
#define PGM_MAGIC "P5"
#define PPM_MAGIC "P6"

#define PAM_MAGIC "P7"

const u8 png_magic[] = {0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a};
const u8 jpg_magic[] = {0xff, 0xd8, 0xff};

typedef enum {
    PNG,
    JPG,
    PAM,
    PBM,
    PGM,
    PPM,
    ASCII_PBM,
    ASCII_PGM,
    ASCII_PPM,
} ImageFormat;

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
    case ASCII_PBM:
        return "ASCII PBM";
    case ASCII_PGM:
        return "ASCII PGM";
    case ASCII_PPM:
        return "ASCII PPM";
    default:
        return "unknown";
    }
}

static ImageFormat guess_image_format(i32 fin) {
    if (is_png(fin)) {
        return PNG;
    }
    if (is_jpg(fin)) {
        return JPG;
    }

    char buf[2];
    read(fin, buf, 2);
    lseek(fin, 0, SEEK_SET);

    if (0 == strncmp(buf, PBM_MAGIC, 2)) {
        return PBM;
    }
    if (0 == strncmp(buf, PGM_MAGIC, 2)) {
        return PGM;
    }
    if (0 == strncmp(buf, PPM_MAGIC, 2)) {
        return PPM;
    }

    if (0 == strncmp(buf, ASCII_PBM_MAGIC, 2)) {
        return ASCII_PBM;
    }
    if (0 == strncmp(buf, ASCII_PGM_MAGIC, 2)) {
        return ASCII_PGM;
    }
    if (0 == strncmp(buf, ASCII_PPM_MAGIC, 2)) {
        return ASCII_PPM;
    }

    if (0 == strncmp(buf, PAM_MAGIC, 2)) {
        return PAM;
    }

    die("Error: unknown format\n");
}

void process_image(const char *input_path, const char *output_path,
                   const Algorithm *algorithms, u32 algorithms_count) {
    i32 fin = open(input_path, O_RDONLY);

    ImageFormat format = guess_image_format(fin);

    close(fin);
}
