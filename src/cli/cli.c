#include <string.h>

#include "../npeg/npeg.h"
#include "cli.h"
#include "die.h"
#include "src/npeg/img/img.h"

void process_image(Arg *args, i32 args_count) {
    char *input_path = NULL;
    char *output_path = NULL;

    for (i32 i = 0; i < args_count; i++) {
        if (0 == strcmp("input", args[i].full_name)) {
            input_path = (char *)args[i].data;
        }
        if (0 == strcmp("output", args[i].full_name)) {
            output_path = (char *)args[i].data;
        }
    }

    ImgResult img_result = img_read(input_path);

    if (img_result.type == FILE_DOES_NOT_EXISTS) {
        die("Error: cannot open image. File '%s' does not exists.\n",
            input_path)
    }

    if (img_result.type == DO_NOT_HAVE_READ_PERMISSIONS) {
        die("Error: cannot open image. Permission denied '%s'.\n", input_path)
    }

    if (img_result.type == UNKNOWN_IMG_TYPE) {
        die("Error: cannot open image. Img type of '%s' is unknown.\n",
            input_path)
    }

    if (img_result.type == IMG_TYPE_IS_NOT_SUPPORTED_YET) {
        die("Error: cannot open image. Img type of '%s' is not supported "
            "yet.\n",
            input_path)
    }

    Img *img = img_result.data;

    for (i32 i = 0; i < args_count; i++) {
        if (0 == strcmp("rotate", args[i].full_name) && args[i].data != NULL) {
            img_rotate(img, *(f32 *)args[i].data, INTER_LINIAR);
        }
    }

    img_write(img, output_path);
    img_free(img);
}

void get_help(Arg *args, i32 args_count) { printf("Help\n"); }
