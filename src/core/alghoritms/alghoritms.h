#ifndef ALGHORITMS_H
#define ALGHORITMS_H

#include "../math/matrix.h"
#include "../options.h"

enum alghoritms {
    VERTICAL_FLIP,
    HORIZONTAL_FLIP,
    ROTATE_90,
    ROTATE_180,
    ROTATE_270,
    BOX_FILTER_ALGHORITM,
    MED_FILTER_ALGHORITM,
    GAUSSIAN_FILTER_ALGHORITM,
    UNSHARP_FILTER_ALGHORITM,
};

enum alghoritms get_alghoritm_type(const char *alghoritm);
void process_image(void *channels, int channels_count, int max_color_value,
                   Options *options);

#endif // ALGHORITMS_H
