
#include "note.h"
#include "filesystem.h"

cJSON* file_to_json( const char* filename ) {
	char* json_string = read_file_content( filename );
	cJSON* doc = cJSON_Parse(json_string);
	return doc;
}

