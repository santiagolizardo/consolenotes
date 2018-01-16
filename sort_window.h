#pragma once

#include "note_link.h"

typedef enum {
	SORT_MODE_CASCADE = 0,
	SORT_MODE_MOSAIC,
	SORT_MODE_RANDOM
} SortMode;

SortMode show_sort_window();

