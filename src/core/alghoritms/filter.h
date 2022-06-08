#ifndef FILTER_H
#define FILTER_H

#include "../math/matrix.h"

enum filters { BOX_FILTER, MED_FILTER, GAUSSIAN_FILTER, UNSHARP_FILTER };

void filter_normal_image(u8_matrix *matrix, int max_color_value,
                         int filter_size, enum filters filter, float sigma);
void filter_extended_image(u16_matrix *matrix, int max_color_value,
                           int filter_size, enum filters filter, float sigma);

#endif // FILTER_H
