cmake_minimum_required(VERSION 3.14)
project(pam_simple)

find_package(OpenSSL REQUIRED)

add_library(pam_simple SHARED
    src/utils/error_handling.c
    src/parsers/argv_parser.c
    src/parsers/shadowfile_parser.c
    src/auth.c
    src/pam_simple.c
)

set_target_properties(pam_simple PROPERTIES POSITION_INDEPENDENT_CODE ON PREFIX "")

target_link_libraries(pam_simple OpenSSL::SSL)

