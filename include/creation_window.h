#pragma once

#include "note.h"
#include "note_link.h"

void create_new_note(NoteLink** selected_link, NoteLink** note_list_head, NoteLink* note_list_tail);
Note* show_create_window(void);

