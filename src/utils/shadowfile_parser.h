#pragma once
#include <shadow.h>

struct spwd* get_shadow_entry(const char* username);
