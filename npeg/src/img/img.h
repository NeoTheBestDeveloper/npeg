#ifndef H_IMG
#define H_IMG

#include "../algorithms/interpolation.h"
#include "img/img_type.h"

#ifndef _IMG_H

typedef void Img;

#endif // !_IMG_H

typedef enum {
    IMG_OK = 0,
    FILE_DOES_NOT_EXISTS = 1,
    DO_NOT_HAVE_WRITE_PERMISSIONS = 2,
    DO_NOT_HAVE_READ_PERMISSIONS = 3,
    UNKNOWN_IMG_TYPE = 4,
    IMG_TYPE_IS_NOT_SUPPORTED_YET = 5,
} ImgResultType;

typedef struct {
    void *data;
    ImgResultType type;
} ImgResult;

ImgResult img_read(const char *path);
ImgResult img_write(const Img *img, const char *path);

Img *img_copy(const Img *img);
void img_free(Img *img);

void img_rotate(Img *img, f32 degrees, Interpolation inter);

inline i64 img_width(const Img *img);
inline i64 img_height(const Img *img);
inline u8 img_depth(const Img *img);
inline u8 img_channels_count(const Img *img);
inline ImgType img_type(const Img *img);

#endif // !H_IMG
