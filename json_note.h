#pragma once

#include <stddef.h>
#include "vendor/cJSON/cJSON.h"

#include "note.h"
#include "note_ui.h"
#include "ui.h"

extern Dimension screen_size;

cJSON* file_to_json( const char* filename );

cJSON* note_to_json( const Note* note, const Point position );

NoteWindow* json_to_note( cJSON* json );

NoteWindow** json_to_list_node( cJSON* doc, int* notes_len );

