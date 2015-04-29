#pragma once

#include <stddef.h>
#include "vendor/cJSON/cJSON.h"

#include "note.h"
#include "note_ui.h"
#include "ui.h"

cJSON* file_to_json( const char* filename );

cJSON* note_to_json( const Note* note );

Note* json_to_note( cJSON* json );

Note** json_to_list_node( cJSON* doc, int* notes_len );

