#pragma once

#include "note.h"

typedef struct NoteLink_ {
  Note *note;
  struct NoteLink_ *prev, *next;
} NoteLink;

NoteLink *new_note_link(void);

void print_all_note_links(const NoteLink *);

size_t count_notes(const NoteLink *note_list_head);

void goto_previous_note(NoteLink **selected_link_ptr, NoteLink *note_list_head);
void goto_next_note(NoteLink **selected_link_ptr, NoteLink *note_list_head);
void archive_current_note(NoteLink *selected_link);
void delete_current_note(NoteLink *selected_link, NoteLink **note_list_head);
