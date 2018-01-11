#include "help_window.h"

#include <string.h>
#include <ncurses.h>

#include "ui.h"

extern Dimension screen_size;

void show_help_window() {
	const struct {
		const char shortcut[5];
		const char description[50];
	} entries[10] = {
		{"c", 		"Create new note"},
		{"e", 		"Edit selected note"},
		{"g", 		"Go to note"},
		{"", 		""},
		{"PgUp", 	"Previus note"},
		{"PgDn", 	"Next note"},
		{"", 		""},
		{"?", 		"This help"},
		{"", 		""},
		{"q", 		"Quit application"},
	};
	const size_t num_entries = 10;

	const Dimension win_size = {
		.w = 40,
		.h = num_entries + 4
	};

	WINDOW *window = newwin(win_size.h, win_size.w, 10, (screen_size.w>>1)-(win_size.w>>1));
	wborder(window, '|', '|', '-', '-', '+', '?', '+', '+' );
	wbkgd(window, COLOR_PAIR(3));

	for(size_t i = 0; i < num_entries; i++) {
		mvwprintw(window, 2 + i, 2, "%-05s %s", entries[i].shortcut, entries[i].description);
	}
	wrefresh(window);

	wtimeout(window, -1);
	wgetch(window);

	werase(window);
	wrefresh(window);

	delwin(window);
}

