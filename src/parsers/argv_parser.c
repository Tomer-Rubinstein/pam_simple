#include "argv_parser.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define ARGV_PARAM_DELIM "="
#define ARGV_PARAM_PERMISSIONS_FILE "permissions"


char *get_argv_key(char *arg, char **rest) {
    arg = strtok_r(arg, ARGV_PARAM_DELIM, rest);
    return arg;
}

char *get_argv_value(char **rest) {
    return strtok_r(NULL, ARGV_PARAM_DELIM, rest);
}

struct acct_argv* parse_acct_argv(int argc, const char **argv) {
    struct acct_argv *parsed_argv = (struct acct_argv *) malloc(sizeof(struct acct_argv));

    char *curr_arg;
    char *rest = NULL;
    for (int i=0; i < argc; i++) {
        curr_arg = (char*) argv[i];

        char *key = get_argv_key(curr_arg, &rest);
        char *value = get_argv_value(&rest);

        if (strcmp(key, ARGV_PARAM_PERMISSIONS_FILE) == 0) {
            parsed_argv->permissions_filename = value;
        }

        rest = NULL;
    }

    return parsed_argv;
}
