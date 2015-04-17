
#include "note.h"
#include "vendor/cJSON/cJSON.h"

cJSON* note_to_json( const Note* note ) {
	if(note == NULL) {
		return NULL;
	}
	cJSON* json_note = cJSON_CreateObject();
	cJSON_AddStringToObject(json_note, "title", note->title);
	cJSON_AddStringToObject(json_note, "description", note->body);
	return json_note;
}

Note* json_to_note( cJSON* json ) {
	if(json == NULL) {
		return NULL;
	}
	Note* note = malloc_note();
	note->title = strdup(cJSON_GetObjectItem(json, "title")->valuestring);
	note->body = strdup(cJSON_GetObjectItem(json, "description")->valuestring);
	return note;
}

Note** json_to_list_node( cJSON* doc, int* notes_len ) {
	int i = 0;
	*notes_len = cJSON_GetArraySize(doc);
	Note** notes = (Note**)malloc(sizeof(Note*) * *notes_len);

	for(; i < *notes_len; i++) {
		cJSON* child = cJSON_GetArrayItem(doc, i);
		Note* note = json_to_note(child);
		notes[ i ] = note;
	}

	return notes;
}	

