#include <stdio.h>
#include <stdlib.h>

#include "../../image_formats/pbm_image.h"

static void _rotate90(pbm_image *image) {
    // // Swap width and height.
    // int old_height = image->height;
    // int old_width = image->width;
    // int old_bytes_width = image->bytes_width;
    //
    // image->width = old_height;
    // image->height = old_bytes_width;
    // image->bytes_width = old_height;
    //
    // unsigned char **new_image_data =
    //     (unsigned char **)malloc(sizeof(unsigned char *) * image->height);
    // for (int i = 0; i < image->height; i++) {
    //     new_image_data[i] = (unsigned char *)malloc(image->bytes_width);
    //     for (int j = 0; j < image->bytes_width; j++) {
    //         new_image_data[i][j] = image->image_data[old_height - 2 - i][i];
    //     }
    // }
    //
    // // Free old memory.
    // for (int i = 0; i < old_height; i++)
    //     free(image->image_data[i]);
    // free(image->image_data);
    //
    // image->image_data = new_image_data;
}
void pbm_rotate(pbm_image *image, int degree) {
    _rotate90(image);
    return;
}
