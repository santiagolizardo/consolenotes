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
	note->archived = false;
	note->toggled = false;

	note->window.handler = NULL;
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
	printf( "creation time: %s\n", ctime(&note->creation_ts) );
	printf( "modification time: %s\n", note->modification_ts != -1? ctime(&note->modification_ts) : NULL );
	printf( "title: %s\n", note->title );
	printf( "body: %s\n", note->body );
	printf( "archived? %s\n", format_yesno_value( note->archived ) );
	printf( "toggled? %s\n", format_yesno_value( note->toggled ) );
}

const char* format_yesno_value( bool value ) {
	return value ? "yes" : "no";
}

