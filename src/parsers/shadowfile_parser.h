#pragma once
#include <shadow.h>

typedef enum HASH_ALGO {
    ALGOTYPE_SHA512,
    ALGOTYPE_SHA256,
    ALGOTYPE_MD5
} HASH_ALGO;

struct shadowfile_entry {
    char* username;
    HASH_ALGO algo;
    char* salt;
    char* passw_hash;
    unsigned int expiration_date; // unix time
};

struct shadowfile_entry* get_shadow_entry(const char* username);
