#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "./parsers/argv_parser.h"
#include "./utils/error_handling.h"

bool is_user_banned(const char *username, int argc, const char **argv) {
    struct acct_argv *parsed_argv = parse_acct_argv(argc, argv);
    FILE *permsfile = fopen(parsed_argv->permissions_filename, "r");

    if (permsfile == NULL) {
        log_err("could not open permissions file");
        return false;
    }

    char *curr_line = NULL;
    while (fscanf(permsfile, "%s", curr_line) > 0) {
        if (strcmp(curr_line, username) == 0)
            return true;
    }

    fclose(permsfile);
    return false;
}

