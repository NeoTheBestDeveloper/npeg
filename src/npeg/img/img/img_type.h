#ifndef IMG_TYPE_H
#define IMG_TYPE_H

#include "types.h"

typedef enum {
    UNKNOWN_MAGIC = -1,
    PNG = 0,
    JPG = 1,
    PAM = 2,
    PBM = 3,
    PGM = 4,
    PPM = 5,
    WEBP = 6,
    TIFF = 7,
} ImgType;

/* Return UNKNOWN (-1) if format is unknown. */
ImgType guess_img_type(i32 fin);
const char *img_type_to_text(ImgType type);

#endif // !IMG_TYPE_H
