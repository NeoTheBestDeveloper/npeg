#include "../pbm/pbm.h"

// For python libary, because ctypes cannots cast pointer of parent sructure to
// pointer of children structure.

PbmImg *cast_img_to_pbm(Img *img) { return (PbmImg *)img; }
Img *cast_void_to_img(void *img) { return (Img *)img; }
