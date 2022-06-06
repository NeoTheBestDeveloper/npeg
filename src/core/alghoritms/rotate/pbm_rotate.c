#include <stdio.h>
#include <stdlib.h>

#include "../../image_formats/pbm_image.h"

void pbm_rotate(pbm_image *image, int degree) {
    int old_width = image->width;
    int old_height = image->height;

    if (degree != 180) {
        // Swap width and height.
        image->width = old_height;
        image->height = old_width;
    }

    unsigned char **new_image_data =
        (unsigned char **)malloc(sizeof(unsigned char *) * image->height);
    for (int i = 0; i < image->height; i++) {
        new_image_data[i] = (unsigned char *)malloc(image->width);
        for (int j = 0; j < image->width; j++) {
            if (degree == 90)
                new_image_data[i][j] = image->image_data[old_height - j - 1][i];
            else if (degree == 270)
                new_image_data[i][j] = image->image_data[j][old_width - i - 1];
            else // 180
                new_image_data[i][j] =
                    image->image_data[old_height - i - 1][old_width - j - 1];
        }
    }

    // Free old memory.
    for (int i = 0; i < old_height; i++)
        free(image->image_data[i]);
    free(image->image_data);

    image->image_data = new_image_data;
}
