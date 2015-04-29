#pragma once

#include <stddef.h>
#include "vendor/cJSON/cJSON.h"

#include "note.h"
#include "note_ui.h"
#include "ui.h"
#include "note_link.h"

cJSON* file_to_json( const char* );

cJSON* note_to_json( const Note* );

cJSON* link_list_to_json( const NoteLink* );

Note* json_to_note( cJSON* );

NoteLink* json_to_list_node( cJSON* );

