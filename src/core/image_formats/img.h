#ifndef H_IMG
#define H_IMG

#include "src/core/math/matrix/u16_matrix.h"
#include "src/core/math/matrix/u8_matrix.h"

typedef struct {
    u8_matrix *channels;
    u16 max_colors;
    u8 channels_count;
} u8_img;

typedef struct {
    u16_matrix *channels;
    u16 max_colors;
    u8 channels_count;
} u16_img;

#endif // !H_IMG
