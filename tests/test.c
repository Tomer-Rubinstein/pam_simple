#include <security/pam_appl.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAX_INPUT 100 + 1


int conversation(int num_msg, const struct pam_message **msgs, struct pam_response **resp, void *appdata_ptr) {
    printf("\tconversation function called!\n");

    struct pam_response *responses_arr = (struct pam_response *) malloc(num_msg * sizeof(struct pam_response));
    char curr_input[MAX_INPUT] = { 0 };

    // set response for each message
    for (int i=0; i < num_msg; i++) {
        memset(curr_input, 0, MAX_INPUT);
        responses_arr[i].resp_retcode = 0; // should always be 0

        // message of current request from PAM module
        const char *msg = msgs[i]->msg; 
        printf("%s\n", msg);

        if (msgs[i]->msg_style == PAM_PROMPT_ECHO_ON || msgs[i]->msg_style == PAM_PROMPT_ECHO_OFF)
            scanf("%100s", curr_input);

        // copy user input to response
        responses_arr[i].resp = (char *) calloc(sizeof(char), (strlen(curr_input)+1));
        strncpy(responses_arr[i].resp, curr_input, strlen(curr_input));
    }

    // pass response to PAM module
    *resp = responses_arr;

    return PAM_SUCCESS;
}

static struct pam_conv conv = {
    conversation,
    NULL
};

void err_n_die(const char* msg) {
    fprintf(stderr, "[ERROR] %s\n", msg);
    exit(-1);
}

int main() {
    pam_handle_t* handle = NULL;
    const char* service_name = "simple";
    char* username;
    int ret;


    // initialize PAM
    ret = pam_start(service_name, NULL, &conv, &handle);
    if (ret != PAM_SUCCESS)
        err_n_die(pam_strerror(handle, ret));

    // authenticate user
    ret = pam_authenticate(handle, 0);
    if (ret != PAM_SUCCESS)
        err_n_die(pam_strerror(handle, ret));

    // check user privileges
    ret = pam_acct_mgmt(handle, 0);
    if (ret != PAM_SUCCESS)
        err_n_die(pam_strerror(handle, ret));

    // user is valid. greet, and offer password change.
    pam_get_item(handle, PAM_USER, (const void**)&username);
    printf("Welcome %s!\n", username);

    // ret = pam_chauthtok(handle, 0); // 0 - force password change, even for valid tokens
    // if (ret != PAM_SUCCESS)
    //     err_n_die(pam_strerror(handle, ret)); 

    pam_end(handle, ret);
    return 0;
}
