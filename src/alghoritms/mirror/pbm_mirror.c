#include <math.h>
#include <stdio.h>

#include "../../core/image_types/pbm_image.h"

static unsigned char _reverse_bits(unsigned char n) {
    // Reverse bits string.
    unsigned int NO_OF_BITS = sizeof(n) * 8;
    unsigned char mirror_num = 0;
    for (int i = 0; i < NO_OF_BITS; i++) {
        if ((n & (1 << i)))
            mirror_num |= 1 << ((NO_OF_BITS - 1) - i);
    }
    return mirror_num;
}

static void _vertical_mirror_ascii_photo(pbm_image *image) {
    for (int i = 0; i < image->height; i++) {
        for (int j = 0; j < image->width / 2; j++) {
            unsigned char tmp = image->image_data[i][j];
            image->image_data[i][j] =
                image->image_data[i][image->width - j - 1];
            image->image_data[i][image->width - j - 1] = tmp;
        }
    }
}

// TODO: when bytes count is not multiple 8 it fill them and when I mirror image
// TODO: photo is spoiled.
static void _vertical_mirror_raw_photo(pbm_image *image) {
    int width = PIXELS_TO_BYTES_WIDTH(image->width);
    for (int i = 0; i < image->height; i++) {
        for (int j = 0; j < width / 2; j++) {
            unsigned char tmp = image->image_data[i][j];
            image->image_data[i][j] =
                _reverse_bits(image->image_data[i][width - j - 1]);
            image->image_data[i][width - j - 1] = _reverse_bits(tmp);
        }
    }
}

void vertical_pbm_mirror(pbm_image *image) {
    if (image->type == RAW) {
        _vertical_mirror_raw_photo(image);
    } else {
        _vertical_mirror_ascii_photo(image);
    }
}

void horizontal_pbm_mirror(pbm_image *image) { printf("Not implemented"); }
