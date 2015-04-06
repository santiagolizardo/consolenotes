#pragma once

#include "note.h"

typedef struct _NoteListNode {
	Note* note;
	struct _NoteListNode* next;
} NoteListNode;

NoteListNode* add_node( NoteListNode* list, Note* note ) {
	NoteListNode* node = (NoteListNode*)malloc(sizeof(NoteListNode));
	node->note = note;
	node->next = NULL;
	if(list != NULL) {
		list->next = node;
	}
	return node;
}

