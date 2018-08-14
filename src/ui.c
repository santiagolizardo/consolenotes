#include "ui.h"

#include <ncurses.h>

extern Dimension screen_size;

void resize_handler() {
	getmaxyx(stdscr, screen_size.h, screen_size.w);
	wnoutrefresh(stdscr);
	doupdate();
}

