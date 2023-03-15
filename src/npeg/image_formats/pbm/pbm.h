#ifndef H_PBM
#define H_PBM

#include <stdbool.h>

#include "../img.h"

typedef struct {
    Img img;
    bool is_ascii;
} PbmImg;

Img *pbm_img_open(const char *path);

void pbm_img_save(const PbmImg *img, const char *output_path);
void pbm_img_free(PbmImg *img);

#endif // !H_PBM