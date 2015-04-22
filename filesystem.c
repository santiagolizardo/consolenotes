#include "filesystem.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

bool file_exists( const char* filename ) {
	return access( filename, F_OK ) == 0;	
}

void write_file_content( const char* filename, const char* content ) {
	FILE* file = fopen(filename, "w");
	fwrite(content, sizeof(char), strlen(content), file);
	fclose(file);
}

char* read_file_content( const char* filename) {
	char* content = NULL;
	FILE* file = fopen(filename, "r");
	if(file) {
		fseek(file, 0, SEEK_END);
		long length = ftell(file);
		fseek(file, 0, SEEK_SET);
		content = (char*)malloc(sizeof(char)*length);
		if(fread(content, sizeof(char), length, file) == 0) {
			free(content);
			content = NULL;
		}
		fclose(file);
	}
	return content;
}

