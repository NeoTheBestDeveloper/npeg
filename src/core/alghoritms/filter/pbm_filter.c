#include <math.h>
#include <stdlib.h>

// #include "../../image_formats/pbm_image.h"
// #include "./pbm_filter.h"

// static void _get_neibhoods(unsigned char *neibghoods, unsigned char **matrix,
//                            int y, int x, int filter_size) {
//     int filter_padding = (filter_size - 1) / 2;
//     int neibghoods_top = 0;
//     for (int i = x - filter_padding; i < x + filter_padding; i++)
//         for (int j = y - filter_padding; j < y + filter_padding; j++)
//             neibghoods[neibghoods_top++] = matrix[i][j];
// }
//
// static void _box_filter(pbm_image *image, int filter_size) {
//     int filter_padding = (filter_size - 1) / 2;
//     int neibghoods_count = filter_size * filter_size;
//     unsigned char *neibghoods = (unsigned char *)malloc(neibghoods_count);
//     for (int i = filter_padding; i < image->height - filter_padding; i++) {
//         for (int j = filter_padding; j < image->width - filter_padding; j++)
//         {
//             int one_count = 0;
//             int zero_count = 0;
//             _get_neibhoods(neibghoods, image->image_data, i, j, filter_size);
//             for (int k = 0; k < neibghoods_count; k++) {
//                 if (neibghoods[k] == '0')
//                     zero_count++;
//                 else
//                     one_count++;
//             }
//             image->image_data[i][j] = (one_count > zero_count) ? '1' : '0';
//         }
//     }
//     free(neibghoods);
// }
//
// static int _mul_vectors(const double *filter, const unsigned char
// *neibghoods,
//                         size_t size) {
//     int result = 0;
//     for (int i = 0; i < size; i++)
//         result += filter[i] * (neibghoods[i] - '0');
//     return result;
// }
//
// static void _gen_gaussian_filter(double *filter, int filter_size, double dev)
// {
//     int filter_top = 0;
//     int filter_padding = (filter_size - 1) / 2;
//     for (int i = -filter_padding; i < filter_size - filter_padding; i++)
//         for (int j = -filter_padding; j < filter_size - filter_padding; j++)
//             filter[filter_top++] =
//                 (1.0 / (sqrt(2.0 * M_PI) * dev)) *
//                 exp(-(pow(i, 2) + pow(j, 2)) / (2.0 * pow(dev, 2)));
// }
//
// static void _med_filter(pbm_image *image, int filter_size) {
//     // It's work
//     int filter_padding = (filter_size - 1) / 2;
//     int neibghoods_count = filter_size * filter_size;
//     unsigned char *neibghoods = (unsigned char *)malloc(neibghoods_count);
//     for (int i = filter_padding; i < image->height - filter_padding; i++) {
//         for (int j = filter_padding; j < image->width - filter_padding; j++)
//         {
//             int one_count = 0;
//             int zero_count = 0;
//             _get_neibhoods(neibghoods, image->image_data, i, j, filter_size);
//             for (int k = 0; k < neibghoods_count; k++) {
//                 if (neibghoods[k] == '0')
//                     zero_count++;
//                 else
//                     one_count++;
//             }
//             image->image_data[i][j] = (one_count > zero_count) ? '1' : '0';
//         }
//     }
//     free(neibghoods);
// }
//
// static void _gaussian_filter(pbm_image *image, int filter_size) {
//     // It's work
//     int filter_padding = (filter_size - 1) / 2;
//     int neibghoods_count = filter_size * filter_size;
//     double *gaussian_filter = (double *)malloc(filter_size * filter_size);
//     _gen_gaussian_filter(gaussian_filter, filter_size, 1.0);
//     unsigned char *neibghoods = (unsigned char *)malloc(neibghoods_count);
//     for (int i = filter_padding; i < image->height - filter_padding; i++) {
//         for (int j = filter_padding; j < image->width - filter_padding; j++)
//         {
//             _get_neibhoods(neibghoods, image->image_data, i, j, filter_size);
//             double result = _mul_vectors(gaussian_filter, neibghoods,
//                                          filter_size * filter_size);
//             image->image_data[i][j] = (result > 0.5) ? '1' : '0';
//         }
//     }
//     free(neibghoods);
//     free(gaussian_filter);
// }
//
// static void _unsharp_filter(pbm_image *image, int filter_size) {}
//
// void filter_image(pbm_image *image, int filter_size, enum filters filter) {
//     switch (filter) {
//     case BOX_FILTER:
//         _box_filter(image, filter_size);
//         break;
//     case MED_FILTER:
//         _med_filter(image, filter_size);
//         break;
//     case GAUSSIAN_FILTER:
//         _gaussian_filter(image, filter_size);
//         break;
//     case UNSHARP_FILTER:
//         _unsharp_filter(image, filter_size);
//         break;
//     }
// }
