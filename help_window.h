#pragma once

#include "ui.h"

extern Dimension screen_size;

void showHelpWindow() {
	const Dimension win_size = { 20, 40 };
	WINDOW *help_window = newwin(win_size.h, win_size.w, 10, (screen_size.w>>1)-(win_size.w>>1));
	box(help_window, 0, 0);
	wborder(help_window, ACS_VLINE, ACS_VLINE, ACS_HLINE, ACS_HLINE, ACS_ULCORNER, ACS_URCORNER, ACS_LLCORNER, ACS_LRCORNER );
	wbkgd(help_window, COLOR_PAIR(3));
	mvwprintw(help_window, 2, 2, "? This help" );
	mvwprintw(help_window, 3, 2, "q Quit application" );
	//wnoutrefresh(help_window);
	//doupdate();
	//nodelay(help_window, TRUE);
	wrefresh(help_window);
//	refresh();
	wtimeout(help_window, -1);
	wgetch(help_window);
	wclear(help_window);
	werase(help_window);
	wrefresh(help_window);
	delwin(help_window);
//	refresh();
}

