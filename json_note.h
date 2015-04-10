
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
	note->title = cJSON_GetObjectItem(json, "title")->valuestring;
	note->body = cJSON_GetObjectItem(json, "description")->valuestring;
	return note;
}

NoteListNode* json_to_list_node( cJSON* doc, NoteListNode** root, NoteListNode **last ) {
	int i = 0,
	    doc_len = cJSON_GetArraySize(doc);

	for(; i < doc_len; i++) {
		cJSON* child = cJSON_GetArrayItem(doc, i);
		Note* note = json_to_note(child);

		if(*root == NULL) {
			*root = *last = add_node(NULL, note);
		}
		else {
			*last = add_node(*last, note);
		}
	}
}	

