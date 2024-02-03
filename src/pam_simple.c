#include <security/pam_appl.h>
#include <security/pam_modules.h>
#include <security/pam_ext.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#define LOG_GET_USER "[pam_simple] Username: "
#define LOG_GET_TOKEN "[pam_simple] Password: "


void log_err(const char *msg) {
    fprintf(stderr, "[ERROR] %s\n", msg);
}

bool auth_user(const char *username, const char *password) {
    return (strcmp(username, "admin") == 0 && strcmp(password, "admin") == 0);
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

    if (auth_user(username, password)) {
        printf("[pam_simple] login ok, hello %s!\n", username);
        return PAM_SUCCESS;
    }

    return PAM_PERM_DENIED;
}
