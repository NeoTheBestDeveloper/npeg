#ifndef PBM_FILTER_H
#define PBM_FILTER_H

#include "../../image_formats/pbm_image.h"

enum filters { BOX_FILTER, MED_FILTER, GAUSSIAN_FILTER, UNSHARP_FILTER };

void filter_image(pbm_image *image, int filter_size, enum filters filter);

#endif // PBM_FILTER_H
