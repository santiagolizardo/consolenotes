#pragma once

#include "note.h"

typedef struct _NoteListNode {
	Note* note;
	struct _NoteListNode* next;
} NoteListNode;

