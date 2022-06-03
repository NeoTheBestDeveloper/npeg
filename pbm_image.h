#include <stdio.h>

typedef struct pbm_image_t {
    int width, height;
    unsigned char **image_data;
} pbm_image;

pbm_image create_pbm_image(const FILE *image);
void free_pbm_image(pbm_image *image);

static void _parse_pbm_specific_metadata(pbm_image *image_metadata,
                                         const FILE *image);
static char **_read_image_data(const FILE *image);
