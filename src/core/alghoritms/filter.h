#ifndef FILTER_H
#define FILTER_H

#include "../math/matrix.h"

enum filters { BOX_FILTER, MED_FILTER, GAUSSIAN_FILTER, UNSHARP_FILTER };

void filter_image(Matrix *matrix, int filter_size, enum filters filter,
                  float sigma);

#endif // FILTER_H
