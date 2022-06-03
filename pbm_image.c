#include "pbm_image.h"

pbm_image create_pbm_image(const FILE *file);
void free_pbm_image(pbm_image *image);

static void _parse_pbm_header(pbm_image *image, const FILE *file);
static char **_read_image_data(const FILE *file);
