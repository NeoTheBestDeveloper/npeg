#include <ctype.h>
#include <die.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <types.h>
#include <unistd.h>

#include "arg.h"

const u8 required_args_count = 2;

bool required_args_gotten[2] = {
    [INPUT] = false,
    [OUTPUT] = false,
};

const Arg help = {
    .arg = "-h",
    .full_arg = "--help",
    .value_type = NONE,
    .values = NULL,
    .values_count = 0,
};

const Arg input = {
    .arg = "-i",
    .full_arg = "--input",
    .value_type = CONST_PATH,
    .values = NULL,
    .values_count = 0,
};

const Arg output = {
    .arg = "-o",
    .full_arg = "--output",
    .value_type = PATH,
    .values = NULL,
    .values_count = 0,
};

const Arg opts = {
    .arg = "-opts",
    .full_arg = "--options",
    .value_type = ARRAY,
    .values = NULL,
    .values_count = 0,
};

const char *algorithm_values[] = {
    "box_filter", "unsharp_filter", "gaussian_filter",
    "rotate",     "flip",           "resize",
};

const Arg algorithm = {
    .arg = "-a",
    .full_arg = "--algorithm",
    .value_type = STR,
    .values = algorithm_values,
    .values_count = 6,
};

static bool is_file_exists(const char *path) { return 0 == access(path, F_OK); }
static bool can_read(const char *path) { return 0 == access(path, R_OK); }

static bool is_file(const Arg *arg) {
    return (arg->value_type == PATH || arg->value_type == CONST_PATH);
}

bool cmp_args(const Arg *arg, const char *str_arg) {
    return (0 == strcmp(arg->arg, str_arg)) ||
           (0 == strcmp(arg->full_arg, str_arg));
}

static void validate_path(ValueType value_type, const char *path) {
    if (value_type == CONST_PATH && !is_file_exists(path)) {
        die("Error: file '%s' does not exists.\n", path);
    }

    if (value_type == CONST_PATH && !can_read(path)) {
        die("Error: does not have permissions for reading file '%s'.\n", path);
    }
}

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

void is_valid_arg_value(const Arg *arg, const char *value) {
    if (is_file(arg)) {
        validate_path(arg->value_type, value);
    }

    if (arg->value_type == INT) {
        if (!is_int(value)) {
            die("Error: argument value %s must be a integer.\n", value);
        }
    } else if (arg->value_type == UINT) {
        if (!is_uint(value)) {
            die("Error: argument value %s must be a unsigned integer.\n",
                value);
        }
    } else if (arg->value_type == REAL) {
        if (!is_real(value)) {
            die("Error: argument value %s must be a real.\n", value);
        }
    }

    if (arg->values != NULL) {
        bool valid_value = false;
        for (u32 i = 0; i < arg->values_count; i++) {
            if (0 == strcmp(value, arg->values[i])) {
                valid_value = true;
            }
        }
        if (!valid_value) {
            die("Error: invalid argument value '%s'.\n", value);
        }
    }
}
