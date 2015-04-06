#include "string_utils.h"

#include <string.h>
#include <ctype.h>

char* uppercase_string( const char* string ) {
	char* new_string = strdup( string );
	int i = 0;
	size_t string_len = strlen(string);
	for(; i < string_len; i++) {
		new_string[i] = toupper(string[i]);
	}
	return new_string;
}

