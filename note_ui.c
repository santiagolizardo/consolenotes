#include "note_ui.h"

#include <stdlib.h>
#include <string.h>

#include "string_utils.h"

const int window_w = 40;

NoteWindow* create_note_window( const Note* note ) {
	NoteWindow* window;
	window = (NoteWindow*)malloc(sizeof(NoteWindow));
	window->note = note;

	window->position.x = 30;
	window->position.y = 10;

	window->window = newwin(20, window_w, window->position.y, window->position.x);

	int title_len = strlen(window->note->title);
	wattron(window->window, COLOR_PAIR(2));
	char* uppercased_title = uppercase_string(window->note->title);
	int centered_x = ( window_w >> 1 ) - ( title_len >> 1 );
	box(window->window, 0 , 0);
	wbkgd(window->window, COLOR_PAIR(2));

	wattron(window->window, A_BOLD | A_UNDERLINE);
	mvwprintw(window->window, 1, centered_x, uppercased_title);
	wattroff(window->window, A_BOLD | A_UNDERLINE);

	free(uppercased_title);
	mvwprintw(window->window, 3, 2, window->note->body);

	window->has_changed = true;
	return window;
}

void note_window_display( const NoteWindow* window ) {
	mvwin(window->window, window->position.y, window->position.x);
	wnoutrefresh(window->window);
}

void free_note_ui( NoteWindow* window ) {
//	delwin(window);
}

