#include "help_window.h"

#include <string.h>
#include <ncurses.h>

#include "ui.h"

extern Dimension screen_size;

void showHelpWindow() {
	const Dimension win_size = {
		.w = 40,
		.h = 10
	};

	WINDOW *window = newwin(win_size.h, win_size.w, 10, (screen_size.w>>1)-(win_size.w>>1));
	wborder(window, '|', '|', '-', '-', '+', '+', '?', '+' );
	wbkgd(window, COLOR_PAIR(3));

	mvwprintw(window, 2, 2, "c Create new note" );
	mvwprintw(window, 3, 2, "? This help" );
	mvwprintw(window, 5, 2, "q Quit application" );
	wrefresh(window);

	wtimeout(window, -1);
	wgetch(window);

	werase(window);
	wrefresh(window);

	delwin(window);
}

