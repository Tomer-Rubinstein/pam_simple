#include <crypt.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "auth.h"
#include "./utils/error_handling.h"
#include "./parsers/shadowfile_parser.h"


bool cmp_password(char* hash_algo, char* salt, const char* passw, char* shadow_hash) {
    // according to man crypt(3): setting = $ALGO$SALT
    // note that param hash_algo nor salt contain '$'
    // thus the need to craft a new formatted string
    const int len_format_string = 1 + strlen(hash_algo) + 1 + strlen(salt);
    char* setting = (char *) calloc(len_format_string+1, sizeof(char));

    snprintf(setting, len_format_string+1, "$%s$%s", hash_algo, salt);

    char* passw_crypt = crypt(passw, setting);
    if (!passw_crypt) {
        log_err("libcrypt error");
        return false;
    }

    // result of crypt (var passw_crypt) is a shadow file entry i.e. $ALGO$SALT$HASH
    // the following will parse HASH from this result
    char *_hash_algo; // ununsed
    char *_salt; // unused
    char *passw_hash;
    if (!parse_shadow_password(passw_crypt, &_hash_algo, &_salt, &passw_hash)) {
        log_err("could not parse libcrypt result");
        return false;
    }

    bool hash_match = (strcmp(shadow_hash, passw_hash) == 0);
    return hash_match;
}

bool auth_user(struct shadowfile_entry* entry, const char* passw) {
    return cmp_password(entry->hash_algo, entry->salt, passw, entry->passw_hash);
}
