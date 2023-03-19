#ifndef H_PBM
#define H_PBM

#include <stdbool.h>

#include "../img/_img.h"

typedef struct {
    Img img;
    bool is_ascii;
} PbmImg;

Img *pbm_img_read(const char *path);
void pbm_img_write(const PbmImg *img, i32 fout);

Img *pbm_img_copy(const PbmImg *img);

#endif // !H_PBM
