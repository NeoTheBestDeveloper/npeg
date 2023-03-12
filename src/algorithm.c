#include <malloc.h>
#include <stdbool.h>
#include <string.h>
#include <types.h>

#include "algorithm.h"

static AlgorithmOption option_new(const char *key, const char *value) {
    u64 key_len = strlen(key);
    u64 value_len = strlen(value);
    AlgorithmOption opt = {
        .key = (char *)malloc(key_len + 1),
        .value = (char *)malloc(value_len + 1),
    };

    strncpy(opt.key, key, key_len + 1);
    strncpy(opt.value, value, value_len + 1);

    return opt;
}

static void option_free(AlgorithmOption *opt) {
    free(opt->key);
    free(opt->value);
}

Algorithm algorithm_new(const char *name, const char *opts) {
    u64 name_len = strlen(name);
    Algorithm algorithm = {
        .name = malloc(name_len + 1),
        .options = NULL,
        .options_count = 0,
    };

    strncpy(algorithm.name, name, name_len + 1);

    u64 opts_len = strlen(opts);

    char key[1024];
    u64 key_top = 0;
    bool key_filled = false;

    char value[1024];
    u64 value_top = 0;
    bool value_filled = false;
    for (u64 i = 0; i < opts_len; i++) {
        if (opts[i] != ' ' && opts[i] != '=') {
            if (!key_filled) {
                key[key_top++] = opts[i];
            } else {
                value[value_top++] = opts[i];
            }
        }

        if (opts[i] == '=') {
            key_filled = true;
        }

        if (opts[i] == ' ') {
            value_filled = true;
        }

        if (key_filled && value_filled) {
            key[key_top] = '\0';
            value[value_top] = '\0';

            if (algorithm.options) {
                algorithm.options = (AlgorithmOption *)realloc(
                    algorithm.options,
                    sizeof(AlgorithmOption) * (algorithm.options_count + 1));
            } else {
                algorithm.options = (AlgorithmOption *)malloc(
                    sizeof(AlgorithmOption) * (algorithm.options_count + 1));
            }
            algorithm.options[algorithm.options_count++] =
                option_new(key, value);

            key_filled = false;
            value_filled = false;
            key_top = 0;
            value_top = 0;
        }
    }

    return algorithm;
}

void algorithm_free(Algorithm *algorithm) {

    for (u32 i = 0; i < algorithm->options_count; i++) {
        option_free(algorithm->options + i);
    }

    free(algorithm->name);
}
