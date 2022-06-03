#include "pbm_image.h"
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

pbm_image create_pbm_image(FILE *file);
void free_pbm_image(pbm_image *image);

static void _parse_pbm_header(pbm_image *image, FILE *file);
static char **_read_from_ascii(FILE *file);

unsigned char reverse_bits(unsigned char n) {
    unsigned int NO_OF_BITS = sizeof(n) * 8;
    unsigned char reverse_num = 0;
    int i;
    for (i = 0; i < NO_OF_BITS; i++) {
        if ((n & (1 << i)))
            reverse_num |= 1 << ((NO_OF_BITS - 1) - i);
    }
    return reverse_num;
}

static unsigned char **_read_from_row(FILE *file, int height, int width) {
    unsigned char **image_data =
        (unsigned char **)malloc(sizeof(unsigned char *) * height);

    for (int i = 0; i < height; i++) {
        image_data[i] = (unsigned char *)malloc(ceil((double)width / 8.0));
        for (int j = 0; j < ceil((double)width / 8.0); j++) {
            fscanf(file, "%c", image_data[i] + j);
        }
    }

    return image_data;
}

void reverse_ASCII_photo(pbm_image *image, const int _direction) {
    for (int i = 0; i < image->height; i++) {
        for (int j = 0; j < image->width / 2; j++) {
            unsigned char tmp = image->image_data[i][j];
            image->image_data[i][j] =
                image->image_data[i][image->width - j - 1];
            image->image_data[i][image->width - j - 1] = tmp;
        }
    }
}

void reverse_raw_photo(pbm_image *image, const int _direction) {
    int bytes_width = ceil((double)image->width / 8.0);
    for (int i = 0; i < image->height; i++) {
        for (int j = 0; j < bytes_width / 2; j++) {
            unsigned char tmp = image->image_data[i][j];
            image->image_data[i][j] =
                reverse_bits(image->image_data[i][bytes_width - j - 1]);
            image->image_data[i][bytes_width - j - 1] = reverse_bits(tmp);
        }
    }
}

void dump_ascii_photo(pbm_image *image, const char *path) {
    FILE *fout = fopen(path, "wt");
    fprintf(fout, "P1\n%d %d\n", image->width, image->height);

    for (int i = 0; i < image->height; i++) {
        for (int j = 0; j < image->width; j++) {
            fprintf(fout, "%c ", image->image_data[i][j]);
        }
    }
    fclose(fout);
}

void dump_raw_photo(pbm_image *image, const char *path) {
    FILE *fout = fopen(path, "wb");
    fprintf(fout, "P4\n%d %d\n", image->width, image->height);

    for (int i = 0; i < image->height; i++) {
        for (int j = 0; j < ceil((double)image->width / 8.0); j++) {
            fprintf(fout, "%c", image->image_data[i][j]);
        }
    }
    fclose(fout);
}
int main(int argc, char *argv[]) {
    FILE *file = fopen("./sample_5184x3456.pbm", "rb");
    int width, height;
    char magic_num[3];
    fscanf(file, "%2s\n", magic_num);
    fscanf(file, "%d %d\n", &width, &height);
    printf("%s\n", magic_num);
    printf("Width: %d, height: %d\n", width, height);
    unsigned char **image_data = _read_from_row(file, height, width);
    pbm_image image = {width, height, image_data};
    reverse_raw_photo(&image, VERTICAL);
    dump_raw_photo(&image, "out.pbm");

    fclose(file);
    return 0;
}
