#pragma once
#include "utils/parsers/shadowfile_parser.h"

bool auth_user(struct shadowfile_entry* shadow_entry, const char* password);
