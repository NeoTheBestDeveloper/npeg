#include <string.h>

#include "alghoritms.h"

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
    else
        return -1;
}
