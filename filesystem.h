#pragma once

#include <stdbool.h>

bool file_exists( const char* filename );

void write_file_content( const char* filename, const char* content );

char* read_file_content( const char* filename );

