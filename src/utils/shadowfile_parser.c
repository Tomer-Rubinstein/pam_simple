#include <stdlib.h>
#include <stdio.h>
#include <sys/param.h>
#include <sys/types.h>
#include <unistd.h>
#include <grp.h>
#include <stdbool.h>

#include "shadowfile_parser.h"
#include "error_handling.h"

#define SHADOWFILE_PATH "/etc/shadow"
#define SHADOWFILE_MAX_LINE 200

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

struct shadowfile_entry* get_shadow_entry(const char* username) {
    struct shadowfile_entry* entry = (struct shadowfile_entry*) calloc(sizeof(struct shadowfile_entry*), 1);
    FILE* fptr = fopen(SHADOWFILE_PATH, "r");

    if (fptr == NULL) {
        log_err("could not open shadow file");
        return NULL;
    }

    char currline[SHADOWFILE_MAX_LINE+1] = { 0 };
    while(fgets(currline, SHADOWFILE_MAX_LINE, fptr)) {
        printf("%s", currline);
    }

    permanent_privileges_drop();


    fclose(fptr);
    return entry;
};
