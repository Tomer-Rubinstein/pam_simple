#include <openssl/sha.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "auth.h"


bool cmp_password(char* org_passw_hash, const char* passw) {
    unsigned char* passw_hash = (unsigned char*) calloc(sizeof(char), SHA512_DIGEST_LENGTH);
    SHA512((unsigned char*)passw, strlen(passw), passw_hash);

    // TODO: convert passw_hash to ascii 

    // TODO: crypt(password+salt)

    printf("passw_hash: %s\n", passw_hash);

    return (strcmp(org_passw_hash, passw_hash) == 0);
}

bool auth_user(struct shadowfile_entry* entry, const char* passw) {
    return cmp_password(entry->passw_hash, passw);
}
