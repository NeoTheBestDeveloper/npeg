#include "../../image_formats/pbm_image.h"

void vertical_pbm_flip(pbm_image *image) {
    for (int i = 0; i < image->height; i++) {
        for (int j = 0; j < image->width / 2; j++) {
            unsigned char tmp = image->image_data[i][j];

            // just swap pixels from different edges of the image.
            image->image_data[i][j] =
                image->image_data[i][image->width - j - 1];
            image->image_data[i][image->width - j - 1] = tmp;
        }
    }
}

void horizontal_pbm_flip(pbm_image *image) {
    for (int i = 0; i < image->height / 2; i++) {
        for (int j = 0; j < image->width; j++) {
            unsigned char tmp = image->image_data[i][j];

            // just swap pixels from different edges of the image.
            image->image_data[i][j] =
                image->image_data[image->height - i - 1][j];
            image->image_data[image->height - i - 1][j] = tmp;
        }
    }
}
