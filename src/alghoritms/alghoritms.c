#include "alghoritms.h"
#include <string.h>

enum alghoritms get_alghoritm_type(const char *alghoritm) {
    if (0 == strcmp(alghoritm, "vertical_mirror")) {
        return VERTICAL_MIRROR;
    } else if (0 == strcmp(alghoritm, "horizontal_mirror")) {
        return VERTICAL_MIRROR;
    } else {
        return UNKNOWN_ALHORITM;
    }
}
