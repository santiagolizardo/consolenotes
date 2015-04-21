#include "agile.h"

#include <ncurses.h>

#include "ui.h"

extern Dimension screen_size;

void draw_lanes() {
	int margin = screen_size.w >> 2;
	mvwvline(stdscr, 0, margin, ACS_VLINE, LINES);
	mvwvline(stdscr, 0, margin << 1, ACS_VLINE, LINES);
	mvwvline(stdscr, 0, ( margin << 1 ) + margin, ACS_VLINE, LINES);
	wnoutrefresh(stdscr);
}

