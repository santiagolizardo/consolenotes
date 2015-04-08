#pragma once

#include <string.h>

void write_file_content( const char* filename, const char* content ) {
	FILE* file = fopen(filename, "w" );
	fwrite(content, sizeof(char), strlen(content), file);
	fclose(file);
}

char* read_file_content( const char* filename) {
	int len = sizeof(char)*1024*100;
	char* content = (char*)malloc(len);
	FILE* file = fopen( filename, "r");
	memset(content, '\0', len);
	fread(content, sizeof(char), len - 1, file);
	fclose(file);
	return content;
}

