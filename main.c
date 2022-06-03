#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int is_exists_key_option_value(int argc, int key_option_position) {
    return !(key_option_position + 1 >= argc);
}

char *parse_key_option_value(int key_option_position, int argc,
                             const char *key_option_name, char *argv[]) {
    if (is_exists_key_option_value(argc, key_option_position)) {
        return argv[key_option_position + 1];
    } else {
        fprintf(stderr, "Error: %s key option does not have value\n",
                key_option_name);
        exit(-1);
    }
}

void validate_input_path(char *input_path) {
    if (access(input_path, F_OK) != 0) {
        fprintf(stderr, "Error: input file path \"%s\" does not exists.\n",
                input_path);
        exit(-1);
    } else if (access(input_path, R_OK) != 0) {
        fprintf(stderr, "Error: does not has permision to read input file.\n");
        exit(-1);
    }
}

int main(int argc, char *argv[]) {
    // Check arguments exsisting.
    if (argc == 1) {
        fprintf(stderr, "Error: no arguments\n");
        exit(-1);
    }

    if (0 == strcmp(argv[1], "-h") || 0 == strcmp(argv[1], "--help")) {
        printf("Help.\n");
        exit(0);
    }

    char input_path[1024];
    char output_path[1024];
    char algorithm[1024];

    for (int i = 1; i < argc; i++) {
        if (0 == strcmp(argv[i], "-i") || 0 == strcmp(argv[i], "--input")) {
            strcpy(input_path, parse_key_option_value(i, argc, "input", argv));
            i++;
        } else if (0 == strcmp(argv[i], "-o") ||
                   0 == strcmp(argv[i], "--output")) {
            strcpy(output_path,
                   parse_key_option_value(i, argc, "output", argv));
            i++;
        } else if (0 == strcmp(argv[i], "-a") ||
                   0 == strcmp(argv[i], "--algorithm")) {
            strcpy(algorithm,
                   parse_key_option_value(i, argc, "algorithm", argv));
            i++;
        } else {
            fprintf(stderr, "Error: unknown option \"%s\".\n", argv[i]);
            exit(-1);
        }
    }

    validate_input_path(input_path);

    return 0;
}
