#ifndef H_IMG
#define H_IMG

#include "../algorithms/post_process.h"
#include "../math/matrix.h"

typedef enum {
    PNG,
    JPG,
    PAM,
    PBM,
    PGM,
    PPM,
} ImageFormat;

typedef struct {
    Matrix *channels;
    u16 max_colors;
    u8 channels_count;
    ImageFormat format;
} Img;

Img *img_open(const char *path);
void img_free(Img *img);

void img_rotate(Img *img, f32 degrees, PostProcess post_process);
void img_save(const Img *img, const char *path);

#endif // !H_IMG
