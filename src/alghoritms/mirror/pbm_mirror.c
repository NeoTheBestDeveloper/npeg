#include <math.h>
#include <stdio.h>

#include "../../core/image_types/pbm_image.h"

static unsigned char _mirror_bits(unsigned char n) {
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

static void _vertical_mirror_raw_photo(pbm_image *image) {
    int width = PIXELS_TO_BYTES_WIDTH(image->width);
    for (int i = 0; i < image->height; i++) {
        for (int j = 0; j < width / 2; j++) {
            unsigned char tmp = image->image_data[i][j];
            image->image_data[i][j] =
                _mirror_bits(image->image_data[i][width - j - 1]);
            image->image_data[i][width - j - 1] = _mirror_bits(tmp);
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
