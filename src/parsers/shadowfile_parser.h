#pragma once
#include <shadow.h>

typedef enum HASH_ALGO {
    SHA512,
    SHA256,
    MD5
} HASH_ALGO;

struct shadowfile_entry {
    char* username;
    HASH_ALGO algo;
    char* salt;
    char* passw_hash;
    unsigned int expiration_date; // unix time
};

struct shadowfile_entry* get_shadow_entry(const char* username);
