#include "json_note.h"

#include <stdlib.h>
#include <string.h>

#include "filesystem.h"

extern Dimension screen_size;

cJSON* file_to_json( const char* filename ) {
	char* json_string = read_file_content( filename );
	cJSON* doc = cJSON_Parse(json_string);
	return doc;
}

cJSON* note_to_json( const Note* note ) {
	if(note == NULL) {
		return NULL;
	}
	cJSON* json_note = cJSON_CreateObject();
	cJSON_AddNumberToObject(json_note, "creation_ts", note->creation_ts);
	cJSON_AddStringToObject(json_note, "title", note->title);
	if(note->body)
		cJSON_AddStringToObject(json_note, "description", note->body);
	else
		cJSON_AddNullToObject(json_note, "description");
	cJSON_AddNumberToObject(json_note, "x", note->window.position.x);
	cJSON_AddNumberToObject(json_note, "y", note->window.position.y);
	if(note->archived)
		cJSON_AddTrueToObject(json_note, "archived");
	else
		cJSON_AddFalseToObject(json_note, "archived");
	if(note->toggled)
		cJSON_AddTrueToObject(json_note, "toggled");
	else
		cJSON_AddFalseToObject(json_note, "toggled");

	return json_note;
}

Note* json_to_note( cJSON* json ) {
	Note* note = NULL;

	if(json == NULL)
		return note;

	note = new_note(false);
	note->creation_ts = cJSON_GetObjectItem(json, "creation_ts")->valueint;
	note->title = strdup(cJSON_GetObjectItem(json, "title")->valuestring);
	cJSON* json_description = cJSON_GetObjectItem(json, "description");
	note->body = json_description->type == cJSON_NULL ? NULL : strdup( json_description->valuestring );
	note->archived = cJSON_GetObjectItem(json, "archived")->type == cJSON_True;
	cJSON* json_toggled = cJSON_GetObjectItem(json, "toggled");
	note->toggled = json_toggled && json_toggled->type == cJSON_True;

	note->window.position.x = cJSON_GetObjectItem(json, "x")->valueint;
	note->window.position.y = cJSON_GetObjectItem(json, "y")->valueint;

	return note;
}

Note** json_to_list_node( cJSON* doc, int* notes_len ) {
	int i;
	*notes_len = cJSON_GetArraySize(doc);
	Note** notes = (Note**)malloc(sizeof(Note*) * *notes_len);
	if(notes == NULL) {
		perror("malloc");
		exit(1);
	}
	for(i = 0; i < *notes_len; i++) {
		cJSON* child = cJSON_GetArrayItem(doc, i);
		Note* note = json_to_note(child);
		notes[ i ] = note;
	}

	return notes;
}	

