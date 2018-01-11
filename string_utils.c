#include "string_utils.h"

#include <string.h>
#include <ctype.h>
#include <stdlib.h>

char* uppercase_string(const char * const string) {
	size_t string_len = strlen(string);
	char* new_string = (char*)malloc(sizeof(char) * string_len + 1);
	size_t i = 0;
	for(; i < string_len; i++) {
		new_string[i] = toupper(string[i]);
	}
    new_string[i] = '\0';
	return new_string;
}

bool string_is_empty(const char * string) {
	while(*string) {
		if(*string != ' ') {
			return false;
		}
		string++;
	}
	return true;
}

char* trim_string(const char * const string) {
	size_t len = strlen(string);
	int begin = 0;
	while(string[begin] == ' ')
		begin++;
	while(string[len-1] == ' ')
		len--;
	return strndup(string+begin, len-begin);
}

