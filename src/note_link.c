#include "note_link.h"

#include <stdlib.h>
#include <assert.h>

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

void goto_previous_note(NoteLink** selected_link_ptr, NoteLink* note_list_head) {
	NoteLink* selected_link = *selected_link_ptr;
	selected_link->note->focused = false;
	if(selected_link->prev) {
		selected_link = selected_link->prev;
	} else {
		selected_link = note_list_head;
	}
	selected_link->note->focused = true;
}

void goto_next_note(NoteLink** selected_link_ptr, NoteLink* note_list_head) {
	NoteLink* selected_link = *selected_link_ptr;
	selected_link->note->focused = false;
	if(selected_link->next) {
		selected_link = selected_link->next;
	} else {
		selected_link = note_list_head;
	}
	selected_link->note->focused = true;
}

void archive_current_note(NoteLink* selected_link) {
	selected_link->note->archived = true;
}

void delete_current_note(NoteLink* selected_link, NoteLink** note_list_head) {
	assert(selected_link != NULL);

	if(selected_link == *note_list_head) {
		if(selected_link->next) {
			*note_list_head = selected_link->next;
		} else {
			*note_list_head = NULL;
		}
		selected_link = NULL;
		return;
	}
	if(selected_link->prev) {
		selected_link->prev->next = selected_link->next;
	}
	if(selected_link->next) {
		selected_link->next->prev = selected_link->prev;
	}
	selected_link = NULL;
}


