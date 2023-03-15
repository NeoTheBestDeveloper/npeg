#ifndef IMG_MAGICS_H
#define IMG_MAGICS_H

#include <types.h>

#define ASCII_PBM_MAGIC "P1"
#define ASCII_PGM_MAGIC "P2"
#define ASCII_PPM_MAGIC "P3"

#define PBM_MAGIC "P4"
#define PGM_MAGIC "P5"
#define PPM_MAGIC "P6"

#define PAM_MAGIC "P7"

const u8 png_magic[] = {0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a};
const u8 jpg_magic[] = {0xff, 0xd8, 0xff};

#endif // !IMG_MAGICS_H
