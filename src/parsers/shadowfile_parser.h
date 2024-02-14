#pragma once

#include <shadow.h>

#define SHADOWFILE_PATH "/etc/shadow"
#define SHADOWFILE_MAX_LINE 200
#define SHADOW_PASSW_DELIMITER "$"

struct shadowfile_entry {
    char* username;
    char* hash_algo;
    char* salt;
    char* passw_hash;
    unsigned int expiration_date; // unix time
};

struct shadowfile_entry* get_shadow_entry(const char* username);
bool parse_shadow_password(char* password, char** dst_hash_algo, char** dst_salt, char** dst_passw_hash);
