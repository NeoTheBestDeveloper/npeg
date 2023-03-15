#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "core.h"
#include "die.h"
#include "image_formats/img.h"
static bool is_int(const char *s) {
    u64 str_len = strlen(s);

    for (u64 i = 0; i < str_len; i++) {
        if (i == 0 && s[i] == '-')
            continue;
        if (!isdigit(s[i]))
            return false;
    }
    return true;
}

static bool is_uint(const char *s) {
    u64 str_len = strlen(s);

    for (u64 i = 0; i < str_len; i++) {
        if (!isdigit(s[i]))
            return false;
    }
    return true;
}

static bool is_real(const char *s) {
    u64 str_len = strlen(s);
    u8 dots_count = 0;

    for (u64 i = 0; i < str_len; i++) {
        if (i == 0 && s[0] == '-')
            continue;
        if (i == 0 && s[0] == '.')
            continue;

        if (i == 1 && s[1] == '-' && s[0] == '.')
            continue;
        if (i == 1 && s[1] == '.' && s[0] == '-')
            continue;

        if (s[i] == '.' && dots_count == 0) {
            dots_count++;
            continue;
        }

        if (s[i] == '.' && dots_count != 0) {
            return false;
        }

        if (!isdigit(s[i]))
            return false;
    }
    return true;
}

void process_image(const char *input_path, const char *output_path,
                   const Algorithm *algorithms, u32 algorithms_count) {
    Img *img = img_open(input_path);
    for (u32 i = 0; i < algorithms_count; i++) {
        switch (algorithms[i].name) {
        case ROTATE: {
            bool rotated = false;
            for (u32 j = 0; j < algorithms[i].options_count; j++) {
                if (0 == strcmp(algorithms->options[j].key, "degrees")) {
                    if (!is_real(algorithms->options[j].value)) {
                        die("Error: 'degrees' option must be real number.\n",
                            NULL)
                    }
                    img_rotate(img, (f32)atof(algorithms->options[j].value));
                    rotated = true;
                    break;
                }
            }
            if (!rotated) {
                die("Error: rotate algorithm must have 'degrees' option.\n",
                    NULL);
            }
            break;
        }
        default:
            die("Error: unknown algorithm.\n", NULL)
        }
    }

    img_save(img, output_path);
    img_free(img);
}
