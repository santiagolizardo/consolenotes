#pragma once

#include <stdbool.h>

bool file_exists( const char* );

void write_file_content( const char*, const char* );

char* read_file_content( const char* );

