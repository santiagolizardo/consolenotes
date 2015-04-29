#include "note_link.h"

#include <stdlib.h>

NoteLink* new_note_link( void ) {
	NoteLink* note_link = (NoteLink*)malloc(sizeof(NoteLink));
	note_link->prev = note_link->next = NULL;
	note_link->note = NULL;
	return note_link;
}

void print_all_note_links( const NoteLink* list ) {
	const NoteLink* current = list;
	while(current) {
		print_note(current->note);
		current = current->next;
	}
}

void print_note_link( const NoteLink* link ) {
	wattron(stdscr,COLOR_PAIR(5));
	mvprintw( 1, 1, "NoteLink[ prev(%p) next(%p) ]", link->prev, link->next);
	wrefresh(stdscr);
	getch();
}

