#pragma once

#include <stdbool.h>

bool file_exists(const char * const);

void write_file_content(const char * const, const char * const);

char* read_file_content(const char * const);

