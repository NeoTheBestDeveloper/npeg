#include <stdlib.h>

#include "cli/cli.h"
#include "types.h"

#define LEN(arr) ((sizeof arr) / (sizeof arr[0]))

i32 main(i32 argc, char *argv[]) {
    Arg help_arg = arg_new("h", "help", BOOLEAN, true);
    Action help_action = action_new(&help_arg, 1, get_help);

    Arg image_process_arguments[] = {
        arg_new("i", "input", STR, true),
        arg_new("o", "output", STR, true),
        arg_new("r", "rotate", FLOAT, false),
        arg_new("s", "size", STR, false),
        arg_new("a", "aspect", STR, false),
        arg_new("f", "filter", STR, false),
    };

    Action image_process_action = action_new(
        image_process_arguments, LEN(image_process_arguments), process_image);

    Action actions[] = {
        image_process_action,
        help_action,
    };

    ArgsParser args_parser = args_parser_new(actions, LEN(actions), argc, argv);
    args_parser_parse(&args_parser);

    args_parser_run_action(&args_parser);

    args_parser_free(&args_parser);

    return EXIT_SUCCESS;
}
