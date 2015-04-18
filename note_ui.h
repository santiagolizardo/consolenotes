#pragma once

#include "ui.h"
#include "note.h"

#include <ncurses.h>

static const int window_w = 40;
static const int window_h = 20;

typedef struct {
	const Note* note;

	Point position;
	WINDOW* window;

	bool has_changed;
} NoteWindow;

NoteWindow* create_note_window( const Note* note, const Dimension screen_size );
void note_window_display( const NoteWindow* window, bool focused );

