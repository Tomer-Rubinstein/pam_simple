#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/param.h>
#include <sys/types.h>
#include <unistd.h>
#include <grp.h>
#include <stdbool.h>
#include <shadow.h>

#include "shadowfile_parser.h"
#include "../utils/error_handling.h"

#define SHADOWFILE_PATH "/etc/shadow"
#define SHADOWFILE_MAX_LINE 200
#define SHADOW_PASSW_DELIMITER "$"

bool permanent_privileges_drop() {
    /* 
    * since the module binary is running as setuid,
    * the effective group/user IDs is root (0)
    * the real group/user IDs is supposedly the lower privileged user
    */

    gid_t newgid = getgid(), oldgid = getegid();
    uid_t newuid = getuid(), olduid = geteuid();

    if (!olduid)
        setgroups(1, &newgid);

    if (newgid != oldgid)
        if (setregid(newgid, newgid) == -1) {
            log_err("could not drop group privileges");
            return false;
        }

    if (newuid != olduid)
        if (setreuid(newuid, newuid) == -1) {
            log_err("could not drop user privileges");
            return false;
        }
    
    /* verify successful changes */

    // true if I can change my effective group ID back to the privileged user
    // OR my effective group ID didn't switch to the unprivileged user
    bool group_change_failed = (setegid(oldgid) != -1 || getegid() != newgid);
    if (newgid != oldgid && group_change_failed) {
        log_err("check for group changes failed");
        return false;
    }

    bool user_change_failed = (seteuid(olduid) != -1 || geteuid() != newuid);
    if (newuid != olduid && user_change_failed) {
        log_err("check for user changes failed");
        return false;
    }

    return true;
}

bool parse_shadow_password(struct shadowfile_entry* entry, char* password) {
    /*
    * according to "Understanding /etc/shadow file format on Linux" (see README),
    * the format of the password field is as follows ($ - delimiter):
    *   $HASH_ALGO$SALT$HASHED_PASSWORD
    * where HASH_ALGO can be $1 for MD5, $5 for SHA-256 and $6 for SHA-512
    */

    char *hash_algo = strtok(password, SHADOW_PASSW_DELIMITER);
    char *salt = strtok(NULL, SHADOW_PASSW_DELIMITER);
    char *hashed_passw = strtok(NULL, SHADOW_PASSW_DELIMITER);
    if (!hash_algo || !salt || !hashed_passw)
        return false;

    entry->algo = -1;
    entry->salt = salt;
    entry->passw_hash = hashed_passw;

    if (strcmp(hash_algo, "1") == 0)
        entry->algo = ALGOTYPE_MD5;
    if (strcmp(hash_algo, "5") == 0)
        entry->algo = ALGOTYPE_SHA256;
    if (strcmp(hash_algo, "6") == 0)
        entry->algo = ALGOTYPE_SHA512;
    
    bool found_algo = (entry->algo != -1);
    return found_algo;
}

struct shadowfile_entry* get_shadow_entry(const char* username) {
    struct spwd* spwd_entry = getspnam(username); // read from shadow file
    permanent_privileges_drop();

    if (!spwd_entry) {
        log_err("could not access shadow file or no entry for user");
        return NULL;
    }

    struct shadowfile_entry* entry = (struct shadowfile_entry*) calloc(sizeof(struct shadowfile_entry), 1);
    entry->username = spwd_entry->sp_namp;
    entry->expiration_date = spwd_entry->sp_expire;

    // parse salt, algo type and hash(password, salt) from spwd_entry->sp_pwdp (hashed passsword field)
    if(!parse_shadow_password(entry, spwd_entry->sp_pwdp)) {
        log_err("could not parse shadow password");
        return NULL;
    }

    printf("found algo: %d\n", entry->algo);
    printf("found salt: %s\n", entry->salt);
    printf("found hashed password: %s\n", entry->passw_hash);

    return entry;
}
