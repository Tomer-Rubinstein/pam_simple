#pragma once

struct acct_argv {
    char *permissions_filename;
};

struct acct_argv* parse_acct_argv(int argc, const char **argv);
