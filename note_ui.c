#include "note_ui.h"

#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "string_utils.h"
#include "colors.h"

extern Dimension screen_size;

void create_note_window( Note* note ) {
	if(note->window.handler == NULL)
		note->window.handler = newwin(window_size.h, window_size.w, note->window.position.y, note->window.position.x);
}

void randomize_position( Note* note ) {
	note->window.position.x = ( rand() % ( screen_size.w - window_size.w ) );
	note->window.position.y = ( rand() % ( screen_size.h - window_size.h ) );
}

void note_window_display( const Note* note ) {
	if(note->window.handler == NULL) {
		// @todo log error
		return;
	}
	werase(note->window.handler);
	wbkgd(note->window.handler, COLOR_PAIR(note->focused ? COLOR_PAIR_NOTE_FOCUSED : COLOR_PAIR_NOTE_NOT_FOCUSED));
	size_t title_len = strlen(note->title);
	char* uppercased_title = uppercase_string(note->title);
	int centered_x = ( window_size.w >> 1 ) - ( title_len >> 1 );

	wattron(note->window.handler, A_BOLD | A_UNDERLINE);
	mvwprintw(note->window.handler, 1, centered_x, uppercased_title);
	wattroff(note->window.handler, A_BOLD | A_UNDERLINE);
	free(uppercased_title);

	if(note->toggled || note->body == NULL) {
		wresize(note->window.handler, 3, window_size.w);
	}
	else {
		wresize(note->window.handler, window_size.h, window_size.w);
		mvwprintw(note->window.handler, 3, 2, note->body);
	}

	mvwin(note->window.handler, note->window.position.y, note->window.position.x);
	box(note->window.handler, 0 , 0);
	mvwprintw(note->window.handler, 0, 0, "#%d", note->index);
	wnoutrefresh(note->window.handler);
}

