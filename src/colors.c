#include "colors.h"

#include <ncurses.h>

void init_color_schemes( void ) {
	if(can_change_color()) {
		init_color(COLOR_BLACK, 0, 0, 0);
	}

	// Dialogs
	init_pair(COLOR_PAIR_CONFIRMATION_DIALOG, COLOR_WHITE, COLOR_RED);
	init_pair(COLOR_PAIR_INFORMATION_DIALOG, COLOR_BLACK, COLOR_YELLOW);
	init_pair(COLOR_PAIR_HELP_DIALOG, COLOR_WHITE, COLOR_BLUE);
	init_pair(COLOR_PAIR_FORM, COLOR_WHITE, COLOR_GREEN);
	init_pair(COLOR_PAIR_FIELD, COLOR_BLACK, COLOR_WHITE);
	init_pair(COLOR_PAIR_FIELD_FORE, COLOR_BLACK, COLOR_CYAN);

	// Notes
	init_pair(COLOR_PAIR_NOTE_FOCUSED, COLOR_WHITE, COLOR_MAGENTA);
	init_pair(COLOR_PAIR_NOTE_NOT_FOCUSED, COLOR_WHITE, COLOR_YELLOW);
}

