#pragma once

#include "note.h"

typedef struct {
	short x, y;
	const Note* note;
} NoteWindow;

NoteWindow* create_note_window( const Note* note );
void note_window_display( const NoteWindow* window );

