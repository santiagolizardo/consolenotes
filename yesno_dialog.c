#include "yesno_dialog.h"

#include <string.h>
#include <ncurses.h>

#include "ui.h"

extern Dimension screen_size;

bool show_yesno_dialog( const char* message ) {
	bool answer = false;
	Dimension win_size = {
		.w = strlen(message) + 4,
		.h = 5
	};

	WINDOW *window = newwin(win_size.h, win_size.w, (screen_size.h>>1)-(win_size.h>>1), (screen_size.w>>1)-(win_size.w>>1));

	box(window, 0, 0);
	wbkgd(window, COLOR_PAIR(7));

	mvwprintw(window, 2, 2, message );
	wrefresh(window);
	wtimeout(window, 0);
	int ch = getch();
	answer = ch == 'y' || ch == 'Y';

	delwin(window);

	return answer;
}

