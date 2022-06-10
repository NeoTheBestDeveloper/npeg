#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "alghoritms.h"
#include "filter.h"

#include "../math/matrix.h"
#include "../options.h"

enum alghoritms get_alghoritm_type(const char *alghoritm) {
    if (0 == strcmp(alghoritm, "vertical_flip"))
        return VERTICAL_FLIP;
    else if (0 == strcmp(alghoritm, "horizontal_flip"))
        return HORIZONTAL_FLIP;
    else if (0 == strcmp(alghoritm, "rotate90"))
        return ROTATE_90;
    else if (0 == strcmp(alghoritm, "rotate180"))
        return ROTATE_180;
    else if (0 == strcmp(alghoritm, "rotate270"))
        return ROTATE_270;
    else if (0 == strcmp(alghoritm, "box_filter"))
        return BOX_FILTER_ALGHORITM;
    else if (0 == strcmp(alghoritm, "median_filter"))
        return MED_FILTER_ALGHORITM;
    else if (0 == strcmp(alghoritm, "gaussian_filter"))
        return GAUSSIAN_FILTER_ALGHORITM;
    else if (0 == strcmp(alghoritm, "unsharp_filter"))
        return UNSHARP_FILTER_ALGHORITM;
    else
        return -1;
}

void process_image(Matrix *channels, int depth, int max_color_value,
                   Options *options) {
    int filter_size = atoi(options->filter_size);
    float sigma = atof(options->sigma);

    for (int i = 0; i < depth; i++)
        switch (get_alghoritm_type(options->alghoritm)) {
        case VERTICAL_FLIP:
            flip_matrix(channels + i, VERTICAL);
            break;
        case HORIZONTAL_FLIP:
            flip_matrix(channels + i, HORIZONTAL);
            break;
        case ROTATE_90:
            rotate_matrix(channels + i, 90);
            break;
        case ROTATE_180:
            rotate_matrix(channels + i, 180);
            break;
        case ROTATE_270:
            rotate_matrix(channels + i, 270);
            break;
        case BOX_FILTER_ALGHORITM:
            filter_image(channels + i, max_color_value, filter_size, BOX_FILTER,
                         sigma);
            break;
        case MED_FILTER_ALGHORITM:
            filter_image(channels + i, max_color_value, filter_size, MED_FILTER,
                         sigma);
        case GAUSSIAN_FILTER_ALGHORITM:
            filter_image(channels + i, max_color_value, filter_size,
                         GAUSSIAN_FILTER, sigma);
            break;
        case UNSHARP_FILTER_ALGHORITM:
            filter_image(channels + i, max_color_value, filter_size,
                         UNSHARP_FILTER, sigma);
            break;
        }
}
