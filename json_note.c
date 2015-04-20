#include "json_note.h"

#include <stdlib.h>
#include <string.h>

#include "filesystem.h"

cJSON* file_to_json( const char* filename ) {
	char* json_string = read_file_content( filename );
	cJSON* doc = cJSON_Parse(json_string);
	return doc;
}

cJSON* note_to_json( const Note* note, const Point position ) {
	if(note == NULL) {
		return NULL;
	}
	cJSON* json_note = cJSON_CreateObject();
	cJSON_AddStringToObject(json_note, "title", note->title);
	cJSON_AddStringToObject(json_note, "description", note->body);
	cJSON_AddNumberToObject(json_note, "x", position.x);
	cJSON_AddNumberToObject(json_note, "y", position.y);

	return json_note;
}

NoteWindow* json_to_note( cJSON* json ) {
	if(json == NULL) {
		return NULL;
	}
	Note* note = malloc_note();
	note->title = strdup(cJSON_GetObjectItem(json, "title")->valuestring);
	note->body = strdup(cJSON_GetObjectItem(json, "description")->valuestring);
	NoteWindow* nw = create_note_window(note, screen_size);
	nw->position.x = cJSON_GetObjectItem(json, "x")->valueint;
	nw->position.y = cJSON_GetObjectItem(json, "y")->valueint;
	return nw;
}

NoteWindow** json_to_list_node( cJSON* doc, int* notes_len ) {
	int i;
	*notes_len = cJSON_GetArraySize(doc);
	NoteWindow** notes = (NoteWindow**)malloc(sizeof(NoteWindow*) * *notes_len);
	if(notes == NULL) {
		perror("malloc");
		exit(1);
	}
	for(i = 0; i < *notes_len; i++) {
		cJSON* child = cJSON_GetArrayItem(doc, i);
		NoteWindow* note = json_to_note(child);
		notes[ i ] = note;
	}

	return notes;
}	

