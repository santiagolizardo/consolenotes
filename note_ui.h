#pragma once

#include "ui.h"
#include "note.h"

#include <ncurses.h>

static const Dimension window_size = {
	.w = 40,
	.h = 20
};

typedef struct {
	Note* note;

	Point position;
	WINDOW* window;

	bool changed;
	bool collapsed;
} NoteWindow;

NoteWindow* create_note_window( Note* note );
void note_window_display( const NoteWindow* window, bool focused );
void randomize_position( NoteWindow* window );

