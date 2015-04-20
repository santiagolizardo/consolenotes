#pragma once

#include <string.h>

#include "ui.h"

extern Dimension screen_size;

void showHelpWindow() {
	const Dimension win_size = {
		.w = 40,
		.h = 10
	};
	WINDOW *help_window = newwin(win_size.h, win_size.w, 10, (screen_size.w>>1)-(win_size.w>>1));
	box(help_window, 0, 0);
	wborder(help_window, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER );
	chtype a = getbkgd(help_window);
	wbkgd(help_window, COLOR_PAIR(5));
	mvwprintw(help_window, 2, 2, "c Create new note" );
	mvwprintw(help_window, 3, 2, "? This help" );
	mvwprintw(help_window, 5, 2, "q Quit application" );
	wnoutrefresh(help_window);
	doupdate();
	wtimeout(help_window, -1);
	wgetch(help_window);
	wbkgd(help_window, a);
	wclear(help_window);
	werase(help_window);
	wnoutrefresh(help_window);
	doupdate();

	delwin(help_window);
}

