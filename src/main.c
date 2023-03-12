#include <die.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "algorithm.h"
#include "arg.h"
#include "src/core/core.h"

char error_msg[2048];

void get_help(void) { printf("Help\n"); }

bool is_argument(const char *arg) {
    u64 arg_len = strlen(arg);
    if (arg_len > 1 && arg[0] == '-') {
        return true;
    }

    if (arg_len > 2 && arg[0] == '-' && arg[1] == '-') {
        return true;
    }

    return false;
}

bool is_argument_sep(const char *arg) { return (0 == strcmp(arg, "--")); }

char *get_arg_value(const Arg *arg, i32 *i, i32 argc, char *argv[]) {
    if (*i == argc - 1) {
        sprintf(error_msg, "Error: arg '%s' must have a value.\n", argv[*i]);
        die(error_msg);
    }

    if (is_argument_sep(argv[*i + 1])) {
        if (*i + 1 == argc - 1) {
            sprintf(error_msg, "Error: arg '%s' must have a value.\n",
                    argv[*i]);
            die(error_msg);
        }
        is_valid_arg_value(arg, argv[*i + 2]);
        (*i) += 2;
        return argv[*i];
    }

    if (is_argument(argv[*i + 1])) {
        sprintf(error_msg,
                "Error: arg '%s' must have a value. If you mean that '%s' is "
                "argument value add '--' before them.\n",
                argv[*i], argv[*i + 1]);
        die(error_msg);
    }

    is_valid_arg_value(arg, argv[*i + 1]);

    (*i)++;
    if (arg->value_type != ARRAY) {
        return argv[*i];
    }

    char *values = NULL;
    while ((*i < argc - 1) && !is_argument(argv[*i])) {
        if (values == NULL) {
            values = (char *)calloc(strlen(argv[*i]) + 2, 1);
        } else {
            values =
                (char *)realloc(values, strlen(values) + strlen(argv[*i]) + 2);
        }
        strncat(values, argv[*i], strlen(argv[*i]));
        strcat(values, " ");
        (*i)++;
    }

    (*i)--;
    return values;
}

i32 main(i32 argc, char *argv[]) {
    if (argc == 1) { // No cmd arguments
        get_help();
        return EXIT_SUCCESS;
    }

    const char *input_path = NULL;
    const char *output_path = NULL;
    char *algorithm_name = NULL;
    Algorithm *algorithms = NULL;
    u32 algorithms_count = 0;

    for (i32 i = 1; i < argc; i++) {
        if (cmp_args(&help, argv[i])) {
            get_help();
            return EXIT_SUCCESS;

        } else if (cmp_args(&input, argv[i])) {
            input_path = get_arg_value(&input, &i, argc, argv);
            required_args_gotten[INPUT] = true;

        } else if (cmp_args(&output, argv[i])) {
            output_path = get_arg_value(&output, &i, argc, argv);
            required_args_gotten[OUTPUT] = true;

        } else if (cmp_args(&algorithm, argv[i])) {
            algorithm_name = get_arg_value(&algorithm, &i, argc, argv);

        } else if (cmp_args(&opts, argv[i])) {
            if (algorithm_name == NULL) {
                sprintf(error_msg,
                        "Error: '%s' must be after algorithm name.\n", argv[i]);
                die(error_msg);
            }
            char *algorithm_opts = get_arg_value(&opts, &i, argc, argv);
            if (algorithms) {
                algorithms = (Algorithm *)realloc(
                    algorithms, sizeof(Algorithm) * (algorithms_count + 1));
            } else {
                algorithms = (Algorithm *)malloc(sizeof(Algorithm) *
                                                 (algorithms_count + 1));
            }
            algorithms[algorithms_count++] =
                algorithm_new(algorithm_name, algorithm_opts);
            free(algorithm_opts);
            algorithm_name = NULL;

        } else {
            sprintf(error_msg, "Error: unknown argument '%s'.\n", argv[i]);
            die(error_msg);
        }
    }

    for (u8 i = 0; i < required_args_count; i++) {
        if (!required_args_gotten[i]) {
            switch (i) {
            case INPUT:
                sprintf(error_msg, "Error: input arg is required.\n");
                die(error_msg);
                break;
            case OUTPUT:
                sprintf(error_msg, "Error: output arg is required.\n");
                die(error_msg);
                break;
            }
        }
    }

    process_image(input_path, output_path, algorithms, algorithms_count);
    for (u32 i = 0; i < algorithms_count; i++) {
        algorithm_free(algorithms + i);
    }

    return EXIT_SUCCESS;
}
