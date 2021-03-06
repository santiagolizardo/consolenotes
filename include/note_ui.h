#pragma once

#include "note.h"
#include "ui.h"

static const Dimension window_size = {.w = 40, .h = 20};

void create_note_window(Note *);
void note_window_display(const Note *);
void randomize_position(Note *);
