#include <string.h>

#include "alghoritms.h"

enum alghoritms get_alghoritm_type(const char *alghoritm) {
    if (0 == strcmp(alghoritm, "vertical_flip")) {
        return VERTICAL_FLIP;
    } else if (0 == strcmp(alghoritm, "horizontal_flip")) {
        return VERTICAL_FLIP;
    } else {
        return -1;
    }
}
