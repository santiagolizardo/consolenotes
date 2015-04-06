#pragma once

#include <time.h>

struct Note {
	time_t creation_ts;
	time_t modification_ts;
	char *title;
	char *body;
};

typedef struct Note Note;

Note* create_note( const char* title, const char* body );
void delete_note( Note* note );
void print_note( const Note* note );
void draw_note( const Note* note );

