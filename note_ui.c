#include "note_ui.h"

#include <ncurses.h>

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

NoteWindow* create_note_window( const Note* note ) {
	NoteWindow* window;
	window = (NoteWindow*)malloc(sizeof(NoteWindow));
	window->x = 30;
	window->y = 10;
	window->note = note;
	return window;
}

char* uppercase_string( const char* string ) {
	char* new_string = strdup( string );
	for(int i = 0; i < strlen(string); i++) {
		new_string[i] = toupper(string[i]);
	}
	return new_string;
}

void note_window_display( const NoteWindow* window ) {
	const int window_w = 40;
	int title_len = strlen(window->note->title);
	WINDOW *win = newwin(20, window_w, 0, 0);
	//wattron(win, COLOR_PAIR(2));
	box(win, 0 , 0);
	wbkgd(win,COLOR_PAIR(2));
	char* uppercased_title = uppercase_string(window->note->title);
	int centered_x = ( window_w >> 1 ) - ( title_len >> 1 );
	mvwprintw(win, 1, centered_x, uppercased_title);
	mvwprintw(win, 3, 2, window->note->body);
	wnoutrefresh(win);
}

void free_note_ui( NoteWindow* window ) {
//	delwin(window);
}

