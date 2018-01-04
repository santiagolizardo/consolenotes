#include "colors.h"

#include <ncurses.h>

void init_color_schemes( void ) {
	if(can_change_color()) {
		init_color(COLOR_BLACK, 1000, 1000, 600);
	}

	init_pair(1, COLOR_BLACK, COLOR_RED);
	init_pair(2, COLOR_WHITE, COLOR_YELLOW);
	init_pair(3, COLOR_WHITE, COLOR_BLUE);
	init_pair(4, COLOR_BLACK, COLOR_WHITE);
	init_pair(5, COLOR_WHITE, COLOR_MAGENTA);
	init_pair(6, COLOR_BLACK, COLOR_CYAN);
	init_pair(7, COLOR_WHITE, COLOR_RED);
	init_pair(8, COLOR_WHITE, COLOR_WHITE);
}

