#ifndef IMG_MAGICS_H
#define IMG_MAGICS_H

#include "types.h"

#define ASCII_PBM_MAGIC "P1"
#define ASCII_PGM_MAGIC "P2"
#define ASCII_PPM_MAGIC "P3"

#define PBM_MAGIC "P4"
#define PGM_MAGIC "P5"
#define PPM_MAGIC "P6"

#define PAM_MAGIC "P7"

const u8 png_magic[8] = {0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a};
const u8 jpg_magic[3] = {0xff, 0xd8, 0xff};
const u8 tiff_magic_le[4] = {0x49, 0x49, 0x2A, 0x00};
const u8 tiff_magic_be[4] = {0x4D, 0x4D, 0x00, 0x2A};

// Webp magic is '52 49 46 46 xx xx xx xx 57 45 42 50 56 50 38', xx - it is size
// of img.
const u8 webp_magic_first_half[4] = {0x52, 0x49, 0x46, 0x46};
const u8 webp_magic_second_half[4] = {0x57, 0x45, 0x42, 0x50};

#endif // !IMG_MAGICS_H
