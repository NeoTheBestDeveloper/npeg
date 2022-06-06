#ifndef ALGHORITMS_H
#define ALGHORITMS_H

enum alghoritms {
    VERTICAL_FLIP,
    HORIZONTAL_FLIP,
    ROTATE_90,
    ROTATE_180,
    ROTATE_270,
    BOX_FILTER_ALGHORITM,
    MED_FILTER_ALGHORITM,
};

enum alghoritms get_alghoritm_type(const char *alghoritm);

#endif // ALGHORITMS_H
