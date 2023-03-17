#include <stdio.h>
#include <string.h>
#include <sys/time.h>

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

    if (!img) {
        die("Error: cannot open image. File '%s' does not exists.\n",
            input_path)
    }

    for (u64 i = 0; i < args_count; i++) {
        if (0 == strcmp("rotate", args[i].full_name) && args[i].data != NULL) {
            struct timeval stop, start;
            gettimeofday(&start, NULL);
            img_rotate(img, *(f32 *)args[i].data);
            gettimeofday(&stop, NULL);
            printf("Rotation time %.3f sec\n",
                   (f32)(stop.tv_sec - start.tv_sec) +
                       (f32)(stop.tv_usec - start.tv_usec) / 1000000.0f);
        }
    }

    img_save(img, output_path);
    img_free(img);
}

void get_help(Arg *args, u64 args_count) { printf("Help\n"); }
