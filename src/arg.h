#ifndef H_ARG
#define H_ARG

#include <stdbool.h>
#include <types.h>

typedef enum {
    NONE,
    STR,
    INT,
    UINT,
    REAL,
    PATH,
    CONST_PATH,
    ARRAY,
} ValueType;

typedef struct {
    const char *arg;
    const char *full_arg;
    const char **values;
    u32 values_count;
    ValueType value_type;
} Arg;

typedef enum {
    INPUT = 0,
    OUTPUT = 1,
} RequiredArgs;

extern const Arg help;
extern const Arg input;
extern const Arg output;
extern const Arg algorithm;
extern const Arg opts;

extern bool required_args_gotten[2];
extern const u8 required_args_count;

bool cmp_args(const Arg *arg, const char *str_arg);
void is_valid_arg_value(const Arg *arg, const char *value);

#endif // !H_ARG
