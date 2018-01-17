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

size_t count_notes(const NoteLink* note_list_head) {
	size_t count = 0;
	while(note_list_head) {
		count++;
		note_list_head = note_list_head->next;
	}
	return count;
}
