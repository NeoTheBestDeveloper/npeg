#include <stdio.h>

typedef struct pbm_image_t {
    int width, height;
    unsigned char **image_data;
} pbm_image;

enum direction { VERTICAL, HORIZONTAL };

pbm_image create_pbm_image(FILE *image);
void free_pbm_image(pbm_image *image);

static void _parse_pbm_specific_metadata(pbm_image *image_metadata,
                                         FILE *image);
static unsigned char **_read_image_data(FILE *image);
void reverse_photo(pbm_image *image, const int _direction);
void dump_photo(pbm_image *image, const char *path);
