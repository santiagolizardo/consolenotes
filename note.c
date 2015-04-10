#include "note.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

Note* malloc_note() {
	Note* note = (Note*)malloc(sizeof(Note));
	return note;
}

void free_note( Note* note ) {
	free(note);
	note = NULL;
}

Note* create_note( const char* title, const char* body ) {
	Note* note = malloc_note();
	note->creation_ts = time(NULL);
	note->modification_ts = -1;
	note->title = strdup( title );
	note->body = strdup( body );
	return note;
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

