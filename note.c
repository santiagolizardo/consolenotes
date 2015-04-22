#include "note.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

Note* new_note( bool init ) {
	Note* note = (Note*)malloc(sizeof(Note));
	if(init)
		init_note(note, NULL, NULL);
	return note;
}

void init_note( Note* note, const char* title, const char* body ) {
	note->creation_ts = time(NULL);
	note->modification_ts = -1;
	note->title = title ? strdup(title) : NULL;
	note->body = body ? strdup(body) : NULL;
}

void del_note( Note* note ) {
	free(note);
	note = NULL;
}

void print_note( const Note* note ) {
	if(note == NULL) {
		fprintf( stderr, "Attempting to print NULL note\n" );
		return;
	}
	printf( "creation date/time: %s\n", ctime(&note->creation_ts) );
	printf( "modification date/time: %s\n", note->modification_ts != -1? ctime(&note->modification_ts) : NULL );
	printf( "title: %s\n", note->title );
	printf( "body: %s\n", note->body );
}

