#include <openssl/sha.h>
#include "auth.h"

bool auth_user(struct shadowfile_entry* shadow_entry, const char* password) {

    return true;
}

bool cmp_password(char* passw_hash, const char* passw) {

}
