#include <security/pam_appl.h>
#include <security/pam_modules.h>
#include <security/pam_ext.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "./utils/argv_parser.h"
#include "./utils/shadowfile_parser.h"
#include "./utils/error_handling.h"

#define LOG_GET_USER "[pam_simple] Username: "
#define LOG_GET_TOKEN "[pam_simple] Password: "
#define FILE_LINE_SIZE 50


bool auth_user(struct spwd* shadow_entry const char* password) {

    return true;
}

PAM_EXTERN int pam_sm_authenticate(pam_handle_t *handle, int flags, int argc, const char **argv) {
    int ret;
    const char *username = NULL;
    const char *password = NULL;

    ret = pam_get_user(handle, &username, LOG_GET_USER);
    if (ret != PAM_SUCCESS) {
        log_err("could not get username from application");
        return PAM_PERM_DENIED;
    }

    // read shadow file early to drop privileges ASAP
    struct spwd* shadow_entry = get_shadow_entry(username);

    ret = pam_get_authtok(handle, PAM_AUTHTOK, &password, LOG_GET_TOKEN);
    if (ret != PAM_SUCCESS) {
        log_err("could not get token from application");
        return PAM_PERM_DENIED;
    }

    /* PAM_DISALLOW_NULL_AUTHTOK flag means to not accept empty auth tokens */
    if (flags & PAM_DISALLOW_NULL_AUTHTOK) {
        if (password == NULL || strcmp(password, "") == 0) {
            log_err("null auth token is not allowed!");
            return PAM_PERM_DENIED;
        }
    }

    if (auth_user(shadow_entry, password)) {
        printf("[pam_simple] login ok, hello %s!\n", username);
        return PAM_SUCCESS;
    }

    return PAM_PERM_DENIED;
}

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

PAM_EXTERN int pam_sm_acct_mgmt(pam_handle_t *handle, int flags, int argc, const char **argv) {
    const char *username;
    pam_get_item(handle, PAM_USER, (const void **)&username);
    
    return (is_user_banned(username, argc, argv)) ? PAM_PERM_DENIED : PAM_SUCCESS;
}

PAM_EXTERN int pam_sm_setcred(pam_handle_t *pamh, int flags, int argc, const char **argv) {
    return PAM_SUCCESS;
}
