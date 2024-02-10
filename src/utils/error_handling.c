#include "error_handling.h"

void log_err(const char *msg) {
    fprintf(stderr, "[ERROR] %s\n", msg);
}
