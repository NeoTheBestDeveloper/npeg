#ifndef AGRS_PARSER_H
#define AGRS_PARSER_H

#include <stdbool.h>

#include "types.h"

typedef enum {
    INT,
    FLOAT,
    BOOLEAN,
    STR,
} ArgType;

/* Struct for containing data about argument. Required flag mean argmunet
requirements for a certain action. */
typedef struct {
    const char *short_name;
    const char *full_name;
    void *data;
    ArgType arg_type;
    bool is_required;
} Arg;

/* Struct for grouping together several args. ArgsParser will define which
action type user by first agument inside action. */
typedef struct {
    Arg *args;
    void (*handler)(Arg *args, u64 args_count);
    u64 args_count;
} Action;

typedef struct {
    Action *actions;
    u64 actions_count;
    char **argv;
    Action *current_action;
    i32 argc;
} ArgsParser;

Arg arg_new(const char *short_name, const char *full_name, ArgType arg_type,
            bool is_required);

Action action_new(Arg *args, u64 args_count,
                  void (*handler)(Arg *args, u64 args_count));

ArgsParser args_parser_new(Action *actions, u64 actions_count, i32 argc,
                           char **argv);
void args_parser_free(ArgsParser *parser);
void args_parser_parse(ArgsParser *args_parser);
void args_parser_run_action(ArgsParser *args_parser);

#endif // !AGRS_PARSER_H
