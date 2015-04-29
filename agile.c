#include "agile.h"

#include <ncurses.h>

#include "ui.h"

extern Dimension screen_size;

void draw_lanes( void ) {
	int margin = screen_size.w >> 2;
	wattron(stdscr, COLOR_PAIR(4));
	mvwvline(stdscr, 0, margin, ACS_VLINE, LINES);
	mvwvline(stdscr, 0, margin << 1, ACS_VLINE, LINES);
	mvwvline(stdscr, 0, ( margin << 1 ) + margin, ACS_VLINE, LINES);
	wattroff(stdscr, COLOR_PAIR(4));
	wnoutrefresh(stdscr);
}

