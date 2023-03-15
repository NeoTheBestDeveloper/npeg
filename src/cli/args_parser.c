#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "args_parser.h"
#include "die.h"

#define HELP_ADVICE "Try 'npeg --help' for more information.\n"

static void arg_free(Arg *arg) {
    if (arg->arg_type != STR) {
        free(arg->data);
    }
}

static void action_free(Action *action) {
    for (u64 i = 0; i < action->args_count; i++) {
        arg_free(action->args + i);
    }
}

void args_parser_free(ArgsParser *parser) {
    for (u64 i = 0; i < parser->actions_count; i++) {
        action_free(parser->actions + i);
    }
}

Arg arg_new(const char *short_name, const char *full_name, ArgType arg_type,
            bool is_required) {
    Arg arg = {
        .short_name = short_name,
        .full_name = full_name,
        .arg_type = arg_type,
        .is_required = is_required,
        .data = NULL,
    };

    return arg;
}

Action action_new(Arg *args, u64 args_count,
                  void (*handler)(Arg *args, u64 args_count)) {
    Action action = {
        .args = args,
        .handler = handler,
        .args_count = args_count,
    };

    return action;
}

ArgsParser args_parser_new(Action *actions, u64 actions_count, i32 argc,
                           char **argv) {
    ArgsParser args_parser = {
        .actions = actions,
        .argc = argc,
        .argv = argv,
        .actions_count = actions_count,
        .current_action = NULL,
    };

    return args_parser;
}

static bool cmp_arg(Arg arg, const char *str_arg) {
    const char *short_name = arg.short_name;
    const char *full_name = arg.full_name;
    return (0 == strcmp(str_arg + 1, short_name) ||
            0 == strcmp(str_arg + 1, full_name));
}

/* Return -1 if action was not recognized.*/
static i64 guess_action_id(ArgsParser *args_parser) {
    const char *first_arg = args_parser->argv[1];

    for (i64 i = 0; i < args_parser->actions_count; i++) {
        if (cmp_arg(args_parser->actions[i].args[0], first_arg)) {
            return i;
        }
    }
    return -1;
}

static i64 find_arg(const char *str_arg, const Action *action) {
    for (i64 i = 0; i < action->args_count; i++) {
        if (cmp_arg(action->args[i], str_arg)) {
            return i;
        }
    }
    return -1;
}

static bool is_arg(const char *str_arg) { return (str_arg[0] == '-'); }

static void find_unknown_args(ArgsParser *args_parser) {
    for (i32 i = 1; i < args_parser->argc; i++) {
        bool arg_found = false;
        for (u64 j = 0; j < args_parser->actions_count; j++) {
            i64 arg_id =
                find_arg(args_parser->argv[i], args_parser->actions + j);
            if (arg_id != -1) {
                arg_found = true;
            }
        }
        if (!arg_found && is_arg(args_parser->argv[i])) {
            die("Error: unknown argument '%s'.\n%s", args_parser->argv[i],
                HELP_ADVICE)
        }
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

static bool is_float(const char *s) {
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

static void validate_arg_type(const char *str_arg, ArgType arg_type) {
    if (arg_type == INT) {
        if (!is_int(str_arg)) {
            die("Error: argument '%s' must be integer number.\n", str_arg)
        }
    } else if (arg_type == FLOAT) {
        if (!is_float(str_arg)) {
            die("Error: argument '%s' must be real number.\n", str_arg)
        }
    }
}

static void write_arg_data(char *str_arg, Arg *arg) {
    if (arg->arg_type == STR) {
        arg->data = str_arg;
    } else if (arg->arg_type == FLOAT) {
        arg->data = malloc(sizeof(f32));
        *(f32 *)(arg->data) = (f32)atof(str_arg);
    } else if (arg->arg_type == INT) {
        arg->data = malloc(sizeof(i64));
        *(i64 *)(arg->data) = atol(str_arg);
    }
}

static void args_parser_parse_action(ArgsParser *parser) {
    for (i32 i = 1; i < parser->argc; i++) {
        if (is_arg(parser->argv[i])) {
            i64 arg_id = find_arg(parser->argv[i], parser->current_action);
            if (arg_id == -1) {
                die("Error: unknown argument '%s'.\n%s", parser->argv[i],
                    HELP_ADVICE)
            }
            Arg *current_arg = parser->current_action->args + arg_id;

            ArgType arg_type = current_arg->arg_type;

            if (arg_type == BOOLEAN) {
                current_arg->data = malloc(sizeof(bool));
                *(bool *)(current_arg->data) = true;
            } else {
                i++;
                validate_arg_type(parser->argv[i], arg_type);
                write_arg_data(parser->argv[i], current_arg);
            }
        }
    }
}

static void validate_required_args(ArgsParser *args_parser) {
    for (u64 i = 0; i < args_parser->current_action->args_count; i++) {
        if (args_parser->current_action->args[i].data == NULL &&
            args_parser->current_action->args[i].is_required) {
            die("Error: argument '--%s' is_required.\n%s",
                args_parser->current_action->args[i].full_name, HELP_ADVICE)
        }
    }
}

void args_parser_parse(ArgsParser *args_parser) {
    i64 action_id = guess_action_id(args_parser);

    if (action_id == -1) {
        die("Error: unknown arguments order.\n%s", HELP_ADVICE)
    }

    args_parser->current_action = args_parser->actions + action_id;

    args_parser_parse_action(args_parser);
    validate_required_args(args_parser);
}

void args_parser_run_action(ArgsParser *args_parser) {
    args_parser->current_action->handler(
        args_parser->current_action->args,
        args_parser->current_action->args_count);
}
