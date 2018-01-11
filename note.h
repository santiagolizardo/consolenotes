#pragma once

#include <stdbool.h>
#include <time.h>
#include <ncurses.h>

#include "ui.h"

struct Note {
	size_t index;
	time_t creation_ts;
	time_t modification_ts;
	char *title;
	char *body;
	bool archived;
	bool toggled;
	bool focused;

	struct {
		Point position;
		WINDOW* handler;
	} window;
};

typedef struct Note Note;

Note* new_note( bool init );
void init_note( Note* note, const char* title, const char* body );
void del_note( Note* note );

Note* create_note( const char* title, const char* body );
void print_note( const Note* note );
void draw_note( const Note* note );

