#include <security/pam_appl.h>
#include <security/pam_modules.h>
#include <security/pam_ext.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "parsers/argv_parser.h"
#include "parsers/shadowfile_parser.h"
#include "utils/error_handling.h"

#include "auth.h"
#include "account.h"

#define LOG_GET_USER "[pam_simple] Username: "
#define LOG_GET_TOKEN "[pam_simple] Password: "


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
    struct shadowfile_entry* shadow_entry = get_shadow_entry(username);
    if (shadow_entry == NULL)
        return PAM_PERM_DENIED;


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

PAM_EXTERN int pam_sm_acct_mgmt(pam_handle_t *handle, int flags, int argc, const char **argv) {
    const char *username;
    pam_get_item(handle, PAM_USER, (const void **)&username);
    
    return (is_user_banned(username, argc, argv)) ? PAM_PERM_DENIED : PAM_SUCCESS;
}

PAM_EXTERN int pam_sm_setcred(pam_handle_t *pamh, int flags, int argc, const char **argv) {
    return PAM_SUCCESS;
}
