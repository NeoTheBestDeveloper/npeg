#include <string.h>

#include "../npeg/npeg.h"
#include "cli.h"
#include "die.h"

void process_image(Arg *args, u64 args_count) {
    char *input_path = NULL;
    char *output_path = NULL;

    for (u64 i = 0; i < args_count; i++) {
        if (0 == strcmp("input", args[i].full_name)) {
            input_path = (char *)args[i].data;
        }
        if (0 == strcmp("output", args[i].full_name)) {
            output_path = (char *)args[i].data;
        }
    }

    Img *img = img_open(input_path);

    for (u64 i = 0; i < args_count; i++) {
        if (0 == strcmp("rotate", args[i].full_name) && args[i].data != NULL) {
            img_rotate(img, *(f32 *)args[i].data);
        }
    }

    img_save(img, output_path);
    img_free(img);
}

void get_help(Arg *args, u64 args_count) { printf("Help\n"); }