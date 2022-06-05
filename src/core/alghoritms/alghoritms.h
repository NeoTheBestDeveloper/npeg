#ifndef ALGHORITMS_H
#define ALGHORITMS_H

enum alghoritms {
    VERTICAL_FLIP,
    HORIZONTAL_FLIP,
    ROTATE_90,
    ROTATE_180,
    ROTATE_270
};

enum alghoritms get_alghoritm_type(const char *alghoritm);

#endif // ALGHORITMS_H
